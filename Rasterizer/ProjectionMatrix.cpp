#include "pch.h"
#include "ProjectionMatrix.h"

#include <cassert>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Rasterizer
{
	ProjectionMatrix::ProjectionMatrix(int width, int height, float near, float far, float fov)
	{	
		fov *= (float)M_PI / 180.f;
		float tanHalf = tanf(fov / 2.f);
		float range = near - far;
		float ar = (float)(width / height);
		m_projection[0] = 1 / tanHalf * ar;
		m_projection[1] = 1/ tanHalf;
		m_projection[2] = (-near - far) / (range);
		m_homogenizer = (2 * far * near / (range));
	}

	Vec3 ProjectionMatrix::operator*(Vec3 vec3) const
	{
		for (int i = 0; i < 3; i++)
		{
			vec3[i] *= m_projection[i];
		}

		vec3.m_z += 1.f;
		vec3 *= 1 / (vec3.m_z * m_homogenizer);

		return vec3;
	}
}
