#pragma once

#include "Mesh.h"
#include "WindowVertex.h"
#include <vector>

namespace Rasterizer
{
	class WindowTriangle
	{
	public:
		WindowTriangle() = delete;
		WindowTriangle(Triangle const& triangle, std::vector<Vec3> const& coordinateBuffer, std::vector<Vec3> const& normalBuffer, std::vector<Vec2> const& texelBuffer, float alpha = 1.f);
		~WindowTriangle() = default;

		void Clip();
		void ApplyViewport(int width, int height);

		Texture* m_texture = nullptr; // public for opti // TODO: make const


		WindowVertex m_vertices[3];

	private:
		Vec3 Interpolate(Vec3 const& vertexA, Vec3 const& vertexB, float ratioAtoAB);
		void Insert(float x, float y);


		Vec2 m_minimum{};
		Vec2 m_maximum{};
		int _maxX = 0;
		int _minX = 0;
		int _maxY = 0;
		int _minY = 0;

	public:
		int const& m_maxX{_maxX};
		int const& m_minX{_minX};
		int const& m_maxY{_maxY};
		int const& m_minY{_minY};
	};
}
