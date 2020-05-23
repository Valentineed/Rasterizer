#include "pch.h"
#include "WindowTriangle.h"

#include "ClipSpace.h"
#include <cassert>

namespace Rasterizer
{
	Rasterizer::WindowTriangle::WindowTriangle(Triangle const& triangle, std::vector<Vec3> const& coordinateBuffer, std::vector<Vec3> const& normalBuffer, std::vector<Vec2> const& texelBuffer, float alpha) :
		m_vertices{
			{ coordinateBuffer[triangle[0].m_coordinateIndex], normalBuffer[triangle[0].m_normalIndex], texelBuffer[triangle[0].m_texelIndex], Color(triangle[0].m_color, alpha) },
			{ coordinateBuffer[triangle[1].m_coordinateIndex], normalBuffer[triangle[1].m_normalIndex], texelBuffer[triangle[1].m_texelIndex], Color(triangle[1].m_color, alpha) },
			{ coordinateBuffer[triangle[2].m_coordinateIndex], normalBuffer[triangle[2].m_normalIndex], texelBuffer[triangle[2].m_texelIndex], Color(triangle[2].m_color, alpha) } },
		m_texture(triangle.m_texture)
	{
	}

	void WindowTriangle::Clip()
	{
		m_minimum = { 1.f, 1.f };
		m_maximum = { -1.f, -1.f };

		Vec3 buffer[14] = { m_vertices[0].m_position, m_vertices[1].m_position, m_vertices[2].m_position };
		Vec3* outputList = buffer;
		int outputSize = 3;
		Vec3* inputList = &buffer[7];
		int inputSize = 0;


		ClipSpace clip;
		for (ClipSpace::Edge edge : clip)
		{
			Vec3* temp = inputList;
			inputList = outputList;
			outputList = temp;
			inputSize = outputSize;
			outputSize = 0;

			Vec3 const* previousPoint = &inputList[inputSize - 1];
			float PreviousDistance = edge.m_normal.Dot({ *previousPoint, edge.m_point });

			for (int i = 0; i < inputSize; i++)
			{
				Vec3 const* currentPoint{ &inputList[i] };
				float CurrentDistance = edge.m_normal.Dot({ *currentPoint, edge.m_point });
				// TODO: distance = 0;
				if (CurrentDistance >= 0)
				{
					if (PreviousDistance < 0)
					{
						outputList[outputSize] = Interpolate(*previousPoint, *currentPoint, PreviousDistance / (PreviousDistance - CurrentDistance));
						outputSize++;
					}
					outputList[outputSize] = *currentPoint;
					outputSize++;
				}

				else if (PreviousDistance >= 0)
				{
					outputList[outputSize] = Interpolate(*previousPoint, *currentPoint, PreviousDistance / (PreviousDistance - CurrentDistance));
					outputSize++;
				}

				PreviousDistance = CurrentDistance;
				previousPoint = currentPoint;
			}

			if (outputSize == 0)
			{
				return;
			}
		}

		for (int i = 0; i < outputSize; i++)
		{
			Insert(outputList[i].m_x, outputList[i].m_y);
		}
	}
	
	Vec3 WindowTriangle::Interpolate(Vec3 const& vertexA, Vec3 const& vertexB, float ratioAtoAB)
	{
		float ratioBtoAB = 1 - ratioAtoAB;
		return Vec3{
				ratioBtoAB * vertexA.m_x + ratioAtoAB * vertexB.m_x,
				ratioBtoAB * vertexA.m_y + ratioAtoAB * vertexB.m_y,
				ratioBtoAB * vertexA.m_z + ratioAtoAB * vertexB.m_z
			};

	}

	void WindowTriangle::Insert(float x, float y)
	{
		if (x < m_minimum.m_u)
		{
			m_minimum.m_u = x;
		}
		if (x > m_maximum.m_u)
		{
			m_maximum.m_u = x;
		}

		if (y < m_minimum.m_v)
		{
			m_minimum.m_v = y;
		}
		if (y > m_maximum.m_v)
		{
			m_maximum.m_v = y;
		}
	}

	void WindowTriangle::ApplyViewport(int width, int height)
	{
		for (int i = 0; i < 3; i++)
		{
			m_vertices[i].m_windowPosition.m_x = (int)((m_vertices[i].m_position.m_x + 1.f) * 0.5f * width);
			m_vertices[i].m_windowPosition.m_y = (int)((m_vertices[i].m_position.m_y + 1.f) * 0.5f * height);
		}

		_maxX = (int)((m_maximum.m_u + 1.f) * 0.5f * width);
		_maxY = (int)((m_maximum.m_v + 1.f) * 0.5f * height);

		_minX = (int)((m_minimum.m_u + 1.f) * 0.5f * width);
		_minY = (int)((m_minimum.m_v + 1.f) * 0.5f * height);
	}
}