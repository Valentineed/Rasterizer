#pragma once

#include "Screen.h"
#include "Scene.h"
#include "Camera.h"

#include "WindowTriangle.h"

namespace Rasterizer
{
	class Rasterizer
	{
	public:
		Rasterizer() = delete;
		~Rasterizer() = delete;

		static void RenderScene(Scene const& pScene, Screen& screen, Camera const& camera, bool isWireframeMode, bool isCullMode, bool isLightOn);
		static void UpdateBuffer(Entity const& entity, Camera const& camera, std::vector<Light> const& lights);

		static bool IsCulled(WindowTriangle const& triangle, Camera const& camera);
		static bool IsClipped(WindowTriangle const& triangle);

	private:
		static WindowTriangle windowTriangle;

		static std::vector<Vec3> coordinateBuffer;
		static std::vector<Vec3> normalBuffer;
		static std::vector<Vec3> lightBuffer;
	};
}
