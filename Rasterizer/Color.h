#pragma once

namespace Rasterizer
{
	class Color
	{
	public:
		Color() = default;
		Color(float red, float green, float blue, float alpha = 1.f);
		Color(float value);
		Color(Color const& other, float alpha);
		~Color() = default;

		float operator[](size_t index) const;
		float& operator[](size_t index);

		Color operator+(Color const& color);
		Color operator*(float const& alpha);

		float m_red = 0.f;
		float m_green = 0.f;
		float m_blue = 0.f;
		float m_alpha = 1.f;
	};
}