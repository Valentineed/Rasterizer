#include "pch.h"
#include "TransformMatrix.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

namespace Rasterizer
{
	TransformMatrix::TransformMatrix(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l) :
		m_matrix{ a, b, c, d, e, f, g, h, i, j, k, l }
	{
	}

	TransformMatrix TransformMatrix::CreateIdentityMatrix()
	{
		return { 1.f, 0.f, 0.f, 0.f,
				 0.f, 1.f, 0.f, 0.f,
				 0.f, 0.f, 1.f, 0.f };
	}

	TransformMatrix TransformMatrix::CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
	{
		TransformMatrix matrix = CreateScaleMatrix(scale);
		matrix *= CreateZRotationMatrix(rotation.m_z);
		matrix *= CreateYRotationMatrix(rotation.m_y);
		matrix *= CreateXRotationMatrix(rotation.m_x);
		matrix *= CreateTranslationMatrix(position);
		return matrix;
	}

	TransformMatrix TransformMatrix::CreateTranslationMatrix(const Vec3& vec3)
	{
		return { 1.f,	0.f,	0.f,	vec3.m_x,
				 0.f,	1.f,	0.f,	vec3.m_y,
				 0.f,	0.f,	1.f,	vec3.m_z };
	}

	TransformMatrix TransformMatrix::CreateScaleMatrix(const Vec3& vec3)
	{
		return { vec3.m_x,	0.f,		0.f,		0.f,
				 0.f,		vec3.m_y,	0.f,		0.f,
				 0.f,		0.f,		vec3.m_z,	0.f };
	}

	TransformMatrix TransformMatrix::CreateRotationMatrix(const Vec3& vec3)
	{
		TransformMatrix matrix = CreateZRotationMatrix(vec3.m_z);
		matrix *= CreateYRotationMatrix(vec3.m_y);
		matrix *= CreateXRotationMatrix(vec3.m_x);
		return matrix;
	}

	TransformMatrix TransformMatrix::CreateXRotationMatrix(float angle)
	{
		angle *= (float)(M_PI / 180);
		return { 1.f,	0.f,			 0.f,			0.f,
				 0.f,	cosf(angle),	-sinf(angle),	0.f,
				 0.f,	sinf(angle),	 cosf(angle),	0.f };
	}

	TransformMatrix TransformMatrix::CreateYRotationMatrix(float angle)
	{
		angle *= (float)(M_PI / 180);
		return { cosf(angle),	0.f,	sinf(angle),	0.f,
				 0.f,			1.f,	0.f,			0.f,
				-sinf(angle),	0.f,	cosf(angle),	0.f };
	}

	TransformMatrix TransformMatrix::CreateZRotationMatrix(float angle)
	{
		angle *= (float)(M_PI / 180);
		return { cosf(angle),	-sinf(angle),	0.f,	0.f,
				 sinf(angle),	 cosf(angle),	0.f,	0.f,
				 0.f,			 0.f,			1.f,	0.f };
	}

	TransformMatrix TransformMatrix::ExtractRotation()
	{
		float scaleX = sqrtf((*this)[0][0] * (*this)[0][0] + (*this)[0][1] * (*this)[0][1] + (*this)[0][2] * (*this)[0][2]);
		float scaleY = sqrtf((*this)[1][0] * (*this)[1][0] + (*this)[1][1] * (*this)[1][1] + (*this)[1][2] * (*this)[1][2]);
		float scaleZ = sqrtf((*this)[2][0] * (*this)[2][0] + (*this)[2][1] * (*this)[2][1] + (*this)[2][2] * (*this)[2][2]);

		// TODO: confirm division is per row
		return { (*this)[0][0] / scaleX,	(*this)[0][1] / scaleX,	(*this)[0][2] / scaleX,		0.f,
				 (*this)[1][0] / scaleY,	(*this)[1][1] / scaleY,	(*this)[1][2] / scaleY,		0.f,
				 (*this)[2][0] / scaleZ,	(*this)[2][1] / scaleZ,	(*this)[2][2] / scaleZ,		0.f };
	}

	TransformMatrix& TransformMatrix::operator*=(TransformMatrix const& other)
	{
		TransformMatrix product;
		for (size_t rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (size_t columnIndex = 0; columnIndex < 4; columnIndex++)
			{
				for (size_t a = 0; a < 3; a++)
				{
					product[rowIndex][columnIndex] += (*this)[rowIndex][a] * other[a][columnIndex];
				}
			}
			product[rowIndex][3] += (*this)[rowIndex][3];
		}
		*this = product;
		return (*this);
	}

	TransformMatrix TransformMatrix::operator*(TransformMatrix const& other) const
	{
		TransformMatrix product;
		for (size_t rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (size_t columnIndex = 0; columnIndex < 4; columnIndex++)
			{
				for (size_t a = 0; a < 3; a++)
				{
					product[rowIndex][columnIndex] += (*this)[rowIndex][a] * other[a][columnIndex];
				}
			}
			product[rowIndex][3] += (*this)[rowIndex][3];
		}
		return product;
	}

	Vec3 TransformMatrix::operator*(Vec3 const& vec3) const
	{
		Vec3 product;

		for (size_t rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (size_t columnIndex = 0; columnIndex < 3; columnIndex++)
			{
				product[rowIndex] += vec3[columnIndex] * (*this)[rowIndex][columnIndex];
			}
			product[rowIndex] += (*this)[rowIndex][3];
		}
		return product;
	}

	TransformMatrix::row TransformMatrix::operator[](size_t rowIndex)
	{
#ifdef _DEBUG
		assert(rowIndex < 4);
#endif // _DEBUG

		return row(&m_matrix[rowIndex * 4]);
	}

	TransformMatrix::const_row TransformMatrix::operator[](size_t rowIndex) const
	{
#ifdef _DEBUG
		assert(rowIndex < 4);
#endif // _DEBUG

		return const_row(&m_matrix[rowIndex * 4]);
	}

	float& TransformMatrix::row::operator[](size_t columIndex)
	{
#ifdef _DEBUG
		assert(columIndex < 4);
#endif // _DEBUG

		return m_ptr[columIndex];
	}

	float TransformMatrix::const_row::operator[](size_t columIndex) const
	{
#ifdef _DEBUG
		assert(columIndex < 4);
#endif // _DEBUG

		return m_ptr[columIndex];
	}
}
