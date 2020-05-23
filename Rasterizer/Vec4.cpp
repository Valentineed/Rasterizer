#include "pch.h"
#include "Vec4.h"
#include <cmath>
#include <cassert>

namespace Rasterizer
{
	Vec4::Vec4(Vec3 const& vec3, float w) :
		m_x(vec3.m_x),
		m_y(vec3.m_y),
		m_z(vec3.m_z),
		m_w(w)
	{
	}

	Vec4::Vec4(Vec4 const& vecA, Vec4 const& vecB) :
		m_x(vecA.m_x - vecB.m_x),
		m_y(vecA.m_y - vecB.m_y),
		m_z(vecA.m_z - vecB.m_z),
		m_w(vecA.m_w - vecB.m_w)
	{
	}

	Rasterizer::Vec4::Vec4(float x, float y, float z, float w) :
		m_x(x),
		m_y(y),
		m_z(z),
		m_w(w)
	{
	}

	void Vec4::Homogenize()
	{
		if (m_w != 0)
		{
			m_x /= m_w;
			m_y /= m_w;
			m_z /= m_w;
			m_w = 1.f;
		}
	}

	float Vec4::GetMagnitude() const
	{
		Vec4 copy = *this;
		copy.Homogenize();

		return sqrtf((copy.m_x * copy.m_x + copy.m_y * copy.m_y + copy.m_z * copy.m_z));
		// ignoring m_w
	}

	void Vec4::Normalize()
	{
		float magnitude = GetMagnitude();
		m_x = m_x / magnitude;
		m_y = m_y / magnitude;
		m_z = m_z / magnitude;
		// ignoring m_w
	}

	Vec4 Vec4::Cross(Vec4 const& other) const
	{
		return Vec4{ m_y * other.m_z - m_z * other.m_y,
					-(m_x * other.m_z - m_z * other.m_x),
					m_x * other.m_y - m_y * other.m_x };
	}

	float Vec4::Dot(Vec4 const& other) const
	{
		return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
	}

	float Vec4::GetDeterminant(Vec4 const& other) const
	{
		return m_y * other.m_z - m_z * other.m_y -(m_x * other.m_z - m_z * other.m_x) + m_x* other.m_y - m_y * other.m_x;
	}

	Vec4& Vec4::operator=(Vec4 const& other)
	{
		m_w = other.m_w;
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;

		return *this;
	}

	float Vec4::operator[](size_t index) const
	{
		switch (index)
		{
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		case 3:
			return m_w;
		default:
			assert(false); // kill function;
			return 0; // return 0 to remove warning
		}
	}

	float& Vec4::operator[](size_t index)
	{
		switch (index)
		{
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		case 3:
			return m_w;
		default:
			assert(false); // kill function;
			return m_x; // return first value to remove warning
		}
	}

	Vec4 Vec4::operator+(Vec4 const& other) const
	{
		Vec4 sum{
			this->m_x + other.m_x,
			this->m_y + other.m_y,
			this->m_z + other.m_z,
			this->m_w + other.m_w
		};

		return sum;
	}

	Vec4 Vec4::operator+=(Vec4 const& other)
	{
		this->m_x += other.m_x;
		this->m_y += other.m_y;
		this->m_z += other.m_z;
		this->m_w += other.m_w;
		return *this;
	}

	Vec4 Vec4::operator-(Vec4 const& other) const
	{
		Vec4 difference{
			this->m_x - other.m_x,
			this->m_y - other.m_y,
			this->m_z - other.m_z,
			this->m_w - other.m_w
		};

		return difference;
	}

	Vec4 Vec4::operator-=(Vec4 const& other)
	{
		this->m_x -= other.m_x;
		this->m_y -= other.m_y;
		this->m_z -= other.m_z;
		this->m_w -= other.m_w;
		return *this;
	}

	Vec4 Vec4::operator*(float const& scalar) const
	{
		Vec4 product{
			this->m_x * scalar,
			this->m_y * scalar,
			this->m_z * scalar,
			this->m_w * scalar
		};

		return product;
	}

	Vec4 Vec4::operator*=(float const& scalar)
	{
		this->m_x *= scalar;
		this->m_y *= scalar;
		this->m_z *= scalar;
		this->m_w *= scalar;

		return *this;
	}
}