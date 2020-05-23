#pragma once

#include "Vec3.h"

#include <utility>

namespace Rasterizer
{
	class ClipSpace // TODO: single Rasterizer::const_iterator
	{
	public:
		class Edge
		{
		public:
			static Vec3 const s_normalRight;
			static Vec3 const s_normalLeft;
			static Vec3 const s_normalTop;
			static Vec3 const s_normalBottom;
			static Vec3 const s_normalFar;
			static Vec3 const s_normalNear;

			static Vec3 const s_minimum;
			static Vec3 const s_maximum;

			Edge() = delete;
			Edge(Vec3 const& normal, Vec3 const& point) :
				m_normal(normal),
				m_point(point)
			{
			}
			~Edge() = default;

			Vec3 const& m_normal;
			Vec3 const& m_point;

		private:

		};

		class const_iterator
		{
		public:
			const_iterator() = default;
			const_iterator(int index) :
				m_currentIndex(index)
			{
			}
			~const_iterator() = default;

			Edge operator*() const
			{
				switch (m_currentIndex)
				{
				case 0:
					return { Edge::s_normalLeft, Edge::s_minimum };
				case 1:
					return { Edge::s_normalTop, Edge::s_maximum };
				case 2:
					return { Edge::s_normalRight, Edge::s_maximum };
				case 3:
					return { Edge::s_normalBottom, Edge::s_minimum };
				case 4:
					return { Edge::s_normalFar, Edge::s_minimum };
				case 5:
					return { Edge::s_normalNear, Edge::s_maximum };
				}
			}
			bool operator==(const const_iterator& other) const
			{
				if (this == &other)
				{
					return true;
				}
				return this->m_currentIndex == other.m_currentIndex;
			}
			bool operator!=(const const_iterator& other) const
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
			int m_currentIndex = 0;

		};

		static const_iterator begin()
		{
			return const_iterator{};
		}
		static const_iterator end()
		{
			return const_iterator{6};
		}

	private:

	};
}