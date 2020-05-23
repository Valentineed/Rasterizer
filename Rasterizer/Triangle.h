#pragma once

#include "Vertex.h"
#include "Texture.h"

namespace Rasterizer
{
	class Triangle
	{
	public:
		Triangle() = delete;
		Triangle(Vertex first, Vertex second, Vertex third, Texture* texture = nullptr);
		~Triangle() = default;

		Vertex const& operator[](size_t index) const;

		Texture* m_texture = nullptr; // public for opti // TODO: make const

	private:
		Vertex m_vertices[3];
	};
}