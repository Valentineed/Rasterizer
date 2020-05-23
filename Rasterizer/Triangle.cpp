#include "pch.h"
#include "Triangle.h"

#include <cassert>

namespace Rasterizer
{
	Triangle::Triangle(Vertex first, Vertex second, Vertex third, Texture* texture) :
		m_vertices{ first, second, third },
		m_texture(texture)
	{
	}

	Vertex const& Triangle::operator[](size_t index) const
	{
#ifdef _DEBUG
		assert(index < 3);
#endif // _DEBUG

		return m_vertices[index];
	}
}