#pragma once

#include "Color.h"
#include "WindowTriangle.h"
#include "Scene.h"
#include "Camera.h"
namespace Rasterizer
{
	class Screen
	{
	public:
		Screen() = default;
		Screen(size_t width, size_t height);
		~Screen();

		void Resize(unsigned int width, unsigned int height);

		int GetWidth() const;
		int GetHeight() const;
		Color* GetPixelBuffer() const;
		float* GetDepthBuffer() const;

		void SetPixelColor(unsigned int x, unsigned int y, const Color& color) const;

		void Clear() const;
		void DrawTriangle(WindowTriangle const& triangle, std::vector<Light> const& lights, std::vector<Vec3> const& lightBuffer, bool useLight) const;

		void DrawFrame(WindowTriangle const& triangle, std::vector<Light> const& lights, std::vector<Vec3> const& lightBuffer, bool useLight) const;

		void DrawLine(WindowTriangle const& triangle, WindowVertex const& first, WindowVertex const& second, std::vector<Light> const& lights, std::vector<Vec3> const& lightBuffer, bool useLight) const;

	private:
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		unsigned int m_capacity = 0;
		Color* m_pixelBuffer = nullptr;
		float* m_depthBuffer = nullptr;

		Color m_clearColor{ 0, 0, 0 };
	};
}