#pragma once
#include "Vec3.h"

namespace Rasterizer
{
	class Vec4
	{
	public:
		Vec4() = default;
		Vec4(Vec3 const& vec3, float w = 1.f);
		Vec4(Vec4 const& vecA, Vec4 const& vecB);
		Vec4(float x, float y, float z = 0.f, float w = 1.f);
		~Vec4() = default;

		void Homogenize();
		float GetMagnitude() const;
		void Normalize();
		Vec4 Cross(Vec4 const& other) const;
		float Dot(Vec4 const& other) const;
		float GetDeterminant(Vec4 const& other) const;

		Vec4& operator=(Vec4 const& other);
		float operator[](size_t index) const;
		float& operator[](size_t index);
		Vec4 operator+(Vec4 const& other) const;
		Vec4 operator+=(Vec4 const& other);
		Vec4 operator-(Vec4 const& other) const;
		Vec4 operator-=(Vec4 const& other);
		Vec4 operator*(float const& scalar) const;
		Vec4 operator*=(float const& scalar);

		float m_x = 0.f;
		float m_y = 0.f;
		float m_z = 0.f;
		float m_w = 0.f;

	private:
	};
}