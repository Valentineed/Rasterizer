#pragma once

namespace Rasterizer
{
	class Vec2
	{
	public:
		Vec2() = default;
		Vec2(float u, float v);
		~Vec2() = default;

		float operator[](size_t index) const;
		float& operator[](size_t index);

		float m_u;
		float m_v;

	private:
	};
}
