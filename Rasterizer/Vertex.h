#pragma once

#include "Vec2.h"
#include "Color.h"
#include <string>

namespace Rasterizer
{
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(int coordinateIndex);
		Vertex(int coordinateIndex, int normalIndex);
		Vertex(int coordinateIndex, Color color);
		Vertex(int coordinateIndex, int normalIndex, Color color);
		Vertex(int coordinateIndex, int normalIndex, int texelIndex);
		Vertex(int coordinateIndex, Color color, int texelIndex);
		Vertex(int coordinateIndex, int normalIndex, Color color, int texelIndex);
		~Vertex() = default;

		int m_coordinateIndex{};
		Color m_color{ 1.f, 1.f, 1.f };
		int m_texelIndex{};
		int m_normalIndex{};

	private:

	};
}
