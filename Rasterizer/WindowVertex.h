#pragma once

#include "Vecii.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Color.h"
namespace Rasterizer
{
	class WindowVertex
	{
	public:
		WindowVertex() = delete;
		WindowVertex(Vec3 const& position, Vec3 const& normal, Vec2 const& texel, Color const& color);
		~WindowVertex() = default;

		Vec3 const& m_position;
		Vecii m_windowPosition{};
		Color m_color;
		Vec2 const& m_texel;
		Vec3 const& m_normal;

	private:

	};
}