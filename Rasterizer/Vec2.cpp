#include "pch.h"
#include "Vec2.h"

#include <cassert>

namespace Rasterizer
{
	Vec2::Vec2(float u, float v) :
		m_u(u),
		m_v(v)
	{
	}

	float& Vec2::operator[](size_t index)
	{
		switch (index)
		{
		case 0:
			return m_u;
		case 1:
			return m_v;
		default:
			assert(false); // kill function;
			return m_u; // return first value to remove warning
		}
	}

	float Vec2::operator[](size_t index) const
	{
		switch (index)
		{
		case 0:
			return m_u;
		case 1:
			return m_v;
		default:
			assert(false); // kill function;
			return 0; // return 0 to remove warning
		}
	}
}