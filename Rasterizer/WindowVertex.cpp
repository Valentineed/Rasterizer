#include "pch.h"
#include "WindowVertex.h"

namespace Rasterizer
{
	WindowVertex::WindowVertex(Vec3 const& position, Vec3 const& normal, Vec2 const& texel, Color const& color) :
		m_position(position),
		m_normal(normal),
		m_texel(texel),
		m_color(color)
	{
	}
}