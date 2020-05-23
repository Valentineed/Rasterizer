#include "pch.h"
#include "Rasterizer.h"

#include <iostream>

namespace Rasterizer
{
	std::vector<Vec3> Rasterizer::coordinateBuffer;
	std::vector<Vec3> Rasterizer::normalBuffer;
	std::vector<Vec3> Rasterizer::lightBuffer;

	void Rasterizer::RenderScene(Scene const& pScene, Screen& screen, Camera const& camera, bool isWireframeMode, bool isCullMode, bool isLightOn)
	{
		screen.Clear();

		for (Entity const& entity : pScene)
		{
			UpdateBuffer(entity, camera, pScene.GetLight());

			for (Triangle triangle : entity)
			{
				// camera space
				WindowTriangle windowTriangle(triangle, coordinateBuffer, normalBuffer, entity.GetTexelBuffer(), entity.GetAlpha());

				if (isCullMode && IsCulled(windowTriangle, camera))
				{
					continue;
				}

				windowTriangle.Clip();
				windowTriangle.ApplyViewport(screen.GetWidth() - 1, screen.GetHeight() - 1); // -1 because index start at 0
				if (IsClipped(windowTriangle))
				{
					continue;
				}

				// display
				if (isWireframeMode)
				{
					screen.DrawFrame(windowTriangle, pScene.GetLight(), lightBuffer, isLightOn);
				}
				else
				{
					screen.DrawTriangle(windowTriangle, pScene.GetLight(), lightBuffer, isLightOn);
				}
			}
		}
	}

	void Rasterizer::UpdateBuffer(Entity const& entity, Camera const& camera, std::vector<Light> const& lights)
	{
		// build matrix
		Mat4 LocalToWorldMatrix = entity.GetTransformation();
		Mat4 const& WorldToCameraMatrix = camera.GetTransformation();
		Mat4 const& projectionMatrix = camera.GetProjection();

		Mat4 lightMatrix = projectionMatrix * WorldToCameraMatrix;
		Mat4 globalMatrix = lightMatrix * LocalToWorldMatrix;

		// coordinate buffer
		std::vector<Vec3> const& localCoordinateBuffer = entity.GetCoordinateBuffer();
		size_t size = localCoordinateBuffer.size();

		coordinateBuffer.clear();
		coordinateBuffer.reserve(size);

		for (int i = 0; i < size; i++)
		{
			coordinateBuffer.emplace_back(globalMatrix * localCoordinateBuffer[i]);
		}
		//http://www.songho.ca/opengl/gl_projectionmatrix.html

		// normal buffer // TODO: change matrix for normal friendly matrix
		std::vector<Vec3> const& localNormalBuffer = entity.GetNormalBuffer();
		size = localNormalBuffer.size();

		normalBuffer.clear();
		normalBuffer.reserve(size);

		Mat4 normalTransformationMatrix = entity.GetNormalTransformation();
		for (int i = 0; i < size; i++)
		{
			normalBuffer.emplace_back(normalTransformationMatrix * localNormalBuffer[i]);
		}

		size = lights.size();
		lightBuffer.clear();
		lightBuffer.reserve(size);
		for (int i = 0; i < size; i++)
		{
			lightBuffer.emplace_back(lightMatrix * lights[i].GetPosition());
		}
	}

	// TODO: inline
	bool Rasterizer::IsClipped(WindowTriangle const& triangle)
	{
		return triangle.m_minX >= triangle.m_maxX ||
			triangle.m_minY >= triangle.m_maxY;
	}

	bool Rasterizer::IsCulled(WindowTriangle const& triangle, Camera const& camera)
	{
		Vec3 v1{ triangle.m_vertices[1].m_position, triangle.m_vertices[0].m_position };
		Vec3 v2{ triangle.m_vertices[2].m_position, triangle.m_vertices[0].m_position };

		float z = v1.m_x * v2.m_y - v1.m_y * v2.m_x;

		return z < 0;
	}
}