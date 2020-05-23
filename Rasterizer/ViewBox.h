#pragma once

#include "Vec3.h"
#include "WindowVertex.h"

namespace Rasterizer
{
	class ViewBox
	{
	public:
		class Edge
		{
		public:
			Edge() = delete;
			Edge(Vec3 const& point, Vec3 const& normal) :
				m_point(point),
				m_normal(normal)
			{}
			~Edge() = default;

			Vec3 const& m_point;
			Vec3 const& m_normal;
		};

		class const_iterator
		{
		public:
			const_iterator() = delete;
			const_iterator(ViewBox const& viewBox, int index = 0) :
				m_currentIndex(index),
				m_edge{
					{ viewBox.m_maximum, viewBox.m_normalRight },
					{ viewBox.m_maximum, viewBox.m_normalTop },
					{ viewBox.m_maximum, viewBox.m_normalNear },
					{ viewBox.m_minimum, viewBox.m_normalLeft },
					{ viewBox.m_minimum, viewBox.m_normalBottom },
					{ viewBox.m_minimum, viewBox.m_normalFar} }
			{
			}
			~const_iterator() = default;
			Edge const& operator*() const
			{
				return m_edge[m_currentIndex];
			}
			bool operator==(const const_iterator & other) const
			{
				if (this == &other)
				{
					return true;
				}
				return this->m_currentIndex == other.m_currentIndex;
			}
			bool operator!=(const const_iterator & other) const
			{
				return !(*this == other);
			}
			const_iterator& operator++()
			{
				m_currentIndex++;
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator tmp(*this);
				operator++();
				return tmp;
			}
		private:
			int m_currentIndex;
			Edge m_edge[6];
		};

		ViewBox() = delete;
		ViewBox(float right, float left, float top, float bottom, float far, float near);
		~ViewBox() = default;

		float GetRight() const;
		float GetLeft() const;
		float GetTop() const;
		float GetBottom() const;
		float GetFar() const;
		float GetNear() const;

		Vec3 GetNormalRight() const;
		Vec3 GetNormalLeft() const;
		Vec3 GetNormalTop() const;
		Vec3 GetNormalBottom() const;
		Vec3 GetNormalFar() const;
		Vec3 GetNormalNear() const;

		const_iterator begin() const;
		const_iterator end() const;

	private:
		Vec3 m_maximum;
		Vec3 m_minimum;

		Vec3 m_normalRight;
		Vec3 m_normalLeft;
		Vec3 m_normalTop;
		Vec3 m_normalBottom;
		Vec3 m_normalFar;
		Vec3 m_normalNear;
	};
}