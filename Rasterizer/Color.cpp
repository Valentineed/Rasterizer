#include "pch.h"
#include "Color.h"
#include <cassert>

namespace Rasterizer
{
	Color::Color(float red, float green, float blue, float alpha) :
		m_red(red),
		m_green(green),
		m_blue(blue),
		m_alpha(alpha)
	{
	}

	Color::Color(float value)
	{
		m_red = value;
		m_green = value;
		m_blue = value;
		m_alpha = 1.f;
	}

	Color::Color(Color const& other, float alpha) :
		m_red(other.m_red),
		m_green(other.m_green),
		m_blue(other.m_blue),
		m_alpha(other.m_alpha * alpha)
	{
	}

	float Color::operator[](size_t index) const
	{
		switch (index)
		{
		case 0:
			return m_red;
		case 1:
			return m_green;
		case 2:
			return m_blue;
		case 3:
			return m_alpha;
		default:
			assert(false); // kill function;
			return 0; // return 0 to remove warning
		}
	}

	float& Color::operator[](size_t index)
	{
		switch (index)
		{
		case 0:
			return m_red;
		case 1:
			return m_green;
		case 2:
			return m_blue;
		case 3:
			return m_alpha;
		default:
			assert(false); // kill function;
			return m_red; // return first value to remove warning
		}
	}

	Color Color::operator+(Color const& color)
	{
		m_red += color.m_red;
		m_green += color.m_green;
		m_blue += color.m_blue;
		m_alpha = color.m_alpha * (1.f - m_alpha) + m_alpha;

		return *this;
	}

	Color Color::operator*(float const& alpha)
	{
		m_red *= alpha;
		m_green *= alpha;
		m_blue *= alpha;
		m_alpha *= alpha;

		return *this;
	}
}