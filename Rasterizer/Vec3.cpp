#include "pch.h"
#include "Vec3.h"
#include <cmath>
#include <cassert>

namespace Rasterizer
{
	Vec3::Vec3(float x, float y, float z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	Vec3::Vec3(Vec3 const& vecA, Vec3 const& vecB) :
		m_x(vecA.m_x - vecB.m_x),
		m_y(vecA.m_y - vecB.m_y),
		m_z(vecA.m_z - vecB.m_z)
	{
	}

	float Vec3::GetMagnitude() const
	{
		return sqrtf((m_x * m_x + m_y * m_y + m_z * m_z));
	}

	Vec3& Vec3::Normalize()
	{
		float magnitude = GetMagnitude();
		m_x = m_x / magnitude;
		m_y = m_y / magnitude;
		m_z = m_z / magnitude;

		return *this;
	}

	Vec3 Vec3::Cross(Vec3 const& other) const
	{
		return Vec3{ m_y * other.m_z - m_z * other.m_y,
					-(m_x * other.m_z - m_z * other.m_x),
					m_x * other.m_y - m_y * other.m_x };
	}

	float Vec3::Dot(Vec3 const& other) const
	{
		return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
	}

	float& Vec3::operator[](size_t index)
	{
		switch (index)
		{
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		default:
			assert(false); // kill function; // TODO: remove ref (ref = 8 octets / float = 4)
			return m_x; // return first value to remove warning
		}
	}

	float Vec3::operator[](size_t index) const
	{
		switch (index)
		{
		case 0:
			return m_x;
		case 1:
			return m_y;
		case 2:
			return m_z;
		default:
			assert(false); // kill function;
			return 0; // return 0 to remove warning
		}
	}

	Vec3 Vec3::operator+(Vec3 const& vector) const
	{
		Vec3 sum{
			this->m_x + vector.m_x,
			this->m_y + vector.m_y,
			this->m_z + vector.m_z,
		};
		return sum;
	}

	Vec3& Vec3::operator+=(Vec3 const& other)
	{
		this->m_x += other.m_x;
		this->m_y += other.m_y;
		this->m_z += other.m_z;
		return *this;
	}

	Vec3 Vec3::operator-() const
	{
		return { -m_x, -m_y, -m_z };
	}

	Vec3 Vec3::operator-(Vec3 const& other) const
	{
		Vec3 difference{
			this->m_x - other.m_x,
			this->m_y - other.m_y,
			this->m_z - other.m_z
		};
		return difference;
	}

	Vec3 Vec3::operator*(float const& scalar) const
	{
		Vec3 product{
			this->m_x * scalar,
			this->m_y * scalar,
			this->m_z * scalar,
		};
		return product;
	}

	Vec3& Vec3::operator*=(float const& scalar)
	{
		this->m_x *= scalar;
		this->m_y *= scalar;
		this->m_z *= scalar;

		return *this;
	}
}