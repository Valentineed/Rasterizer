#pragma once

namespace Rasterizer
{
	class Vec3
	{
	public:
		Vec3() = default;
		Vec3(float x, float y, float z = 0.f);
		Vec3(Vec3 const& vecA, Vec3 const& vecB);
		~Vec3() = default;

		float GetMagnitude() const;
		Vec3& Normalize();
		Vec3 Cross(Vec3 const& other) const;
		float Dot(Vec3 const& other) const;

		float operator[](size_t index) const;
		float& operator[](size_t index);
		Vec3 operator+(Vec3 const& other) const;
		Vec3& operator+=(Vec3 const& other);
		Vec3 operator-() const;
		Vec3 operator-(Vec3 const& other) const;
		Vec3 operator*(float const& scalar) const;
		Vec3& operator*=(float const& scalar);

		float m_x = 0.f;
		float m_y = 0.f;
		float m_z = 0.f;

	private:
	};
}