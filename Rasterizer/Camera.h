#pragma once

#include "Mat4.h"
#include <utility>

namespace Rasterizer
{
	class Camera
	{
	public:
		Camera() = default;
		~Camera() = default;

		Mat4 const& GetProjection() const;
		Mat4 GetTransformation() const;

		void SetPerspectiveView(int width, int height, float near, float far, float fov);
		void SetOrthographicView(float width, float height, float depth);
		void SetOrthographicView(float right, float left, float top, float bottom, float near, float far);

		Vec3 const& GetPosition() const;
		void SetPosition(Vec3 position);
		void Translate(Vec3 movement);

		void SetRotation(Vec3 rotation);
		void Rotate(Vec3 rotation);

	private:
		Mat4 m_projection{ Mat4::CreateIdentityMatrix() };

		Vec3 m_position = {};
		Vec3 m_rotation = {};
	};
}
