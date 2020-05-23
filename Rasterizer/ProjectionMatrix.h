#pragma once

#include "Vec3.h"

namespace Rasterizer
{
	class ProjectionMatrix
	{
	public:

		ProjectionMatrix() = delete;
		ProjectionMatrix(int width, int height, float near, float far, float fov);
		~ProjectionMatrix() = default;

		Vec3 operator*(Vec3 vec3) const;

	private:
		Vec3 m_projection;
		float m_homogenizer;
	};
}