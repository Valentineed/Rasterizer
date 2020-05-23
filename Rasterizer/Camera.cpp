#include "pch.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Rasterizer
{
	Mat4 const& Camera::GetProjection() const
	{
		return m_projection;
	}

	Mat4 Camera::GetTransformation() const
	{
		Mat4 matrix = Mat4::CreateXRotationMatrix(-m_rotation.m_x);
		matrix *= Mat4::CreateZRotationMatrix(-m_rotation.m_z);
		matrix *= Mat4::CreateYRotationMatrix(-m_rotation.m_y);
		matrix *= Mat4::CreateTranslationMatrix(-m_position);
		return matrix;
	}

	void Camera::SetPerspectiveView(int width, int height, float near, float far, float fov)
	{
		float ar = width / (float)height;

		m_projection = Mat4::CreatePerspectiveMatrix(ar, near, far, fov);
	}

	void Camera::SetOrthographicView(float width, float height, float depth)
	{
		float right = width / 2;
		float top = height / 2;
		float near = depth / 2;

		m_projection = Mat4::CreateOrthographicMatrix(width, height, depth);
	}

	void Camera::SetOrthographicView(float right, float left, float top, float bottom, float near, float far)
	{
		m_projection = Mat4::CreateOrthographicMatrix(right, left, top, bottom, near, far);
	}

	Vec3 const& Camera::GetPosition() const
	{
		return m_position;
	}

	void Camera::SetPosition(Vec3 position)
	{
		m_position = position;
	}

	void Camera::Translate(Vec3 movement)
	{
		if (movement.m_z != 0) // forward move
		{
			float angle = m_rotation.m_y * (float)M_PI / 180.f;
			m_position.m_z += movement.m_z * cosf(angle);
			m_position.m_x += movement.m_z * sinf(angle);
		}
		if (movement.m_x != 0) // move sideway
		{
			float angle = (m_rotation.m_y + 90.f) * (float)M_PI / 180.f;
			m_position.m_z += movement.m_x * cosf(angle);
			m_position.m_x += movement.m_x * sinf(angle);
		}
		m_position.m_y += movement.m_y;
	}

	void Camera::SetRotation(Vec3 rotation)
	{
		m_rotation = rotation;
	}

	void Camera::Rotate(Vec3 rotation)
	{
		m_rotation += rotation;

		if (m_rotation.m_x > 90.f)
		{
			m_rotation.m_x = 90.0f;
		}
		if (m_rotation.m_x < -90.0f)
		{
			m_rotation.m_x = -90.0f;
		}
	}
}
