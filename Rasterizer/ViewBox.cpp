#include "pch.h"
#include "ViewBox.h"

namespace Rasterizer
{
	ViewBox::ViewBox(float right, float left, float top, float bottom, float far, float near) :
		m_maximum({ right, top, far }),
		m_minimum({ left, bottom, near }),
		m_normalBottom({ 0.f, 1.f }),
		m_normalFar({ 0.f, 0.f, 1.f }),
		m_normalLeft({ 1.f, 0.f }),
		m_normalNear({ 0.f, 0.f, -1.f }),
		m_normalRight({ -1.f, 0.f }),
		m_normalTop({ 0.f, -1.f })
	{
	}

	float ViewBox::GetRight() const
	{
		return m_maximum.m_x;
	}

	float ViewBox::GetLeft() const
	{
		return m_minimum.m_x;
	}

	float ViewBox::GetTop() const
	{
		return m_maximum.m_y;
	}

	float ViewBox::GetBottom() const
	{
		return m_minimum.m_y;
	}

	float ViewBox::GetFar() const
	{
		return m_maximum.m_z;
	}

	float ViewBox::GetNear() const
	{
		return m_minimum.m_z;
	}
	Vec3 ViewBox::GetNormalRight() const
	{
		return m_normalRight;
	}
	Vec3 ViewBox::GetNormalLeft() const
	{
		return m_normalLeft;
	}
	Vec3 ViewBox::GetNormalTop() const
	{
		return m_normalTop;
	}
	Vec3 ViewBox::GetNormalBottom() const
	{
		return m_normalBottom;
	}
	Vec3 ViewBox::GetNormalFar() const
	{
		return m_normalFar;
	}
	Vec3 ViewBox::GetNormalNear() const
	{
		return m_normalNear;
	}

	ViewBox::const_iterator ViewBox::begin() const
	{
		return const_iterator(*this);
	}

	ViewBox::const_iterator ViewBox::end() const
	{
		return const_iterator(*this, 6);
	}
}