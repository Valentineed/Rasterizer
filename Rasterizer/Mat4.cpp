#include "pch.h"
#include "Mat4.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

namespace Rasterizer
{
	Mat4::Mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) :
		m_matrix{ a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p }
	{
	}

	Mat4 Mat4::CreateIdentityMatrix()
	{
		return { 1.f, 0.f, 0.f, 0.f,
				 0.f, 1.f, 0.f, 0.f,
				 0.f, 0.f, 1.f, 0.f,
				 0.f, 0.f, 0.f, 1.f };
	}

	Mat4 Mat4::CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
	{
		Mat4 matrix = CreateTranslationMatrix(position);
		matrix *= CreateYRotationMatrix(rotation.m_y);
		matrix *= CreateZRotationMatrix(rotation.m_z);
		matrix *= CreateXRotationMatrix(rotation.m_x);
		matrix *= CreateScaleMatrix(scale);
		return matrix;
	}

	Mat4 Mat4::CreateTranslationMatrix(const Vec3& vec3)
	{
		return { 1.f,	0.f,	0.f,	vec3.m_x,
				 0.f,	1.f,	0.f,	vec3.m_y,
				 0.f,	0.f,	1.f,	vec3.m_z,
				 0.f,	0.f,	0.f,	1.f };
	}

	Mat4 Mat4::CreateScaleMatrix(const Vec3& vec3)
	{
		return { vec3.m_x,	0.f,		0.f,		0.f,
				 0.f,		vec3.m_y,	0.f,		0.f,
				 0.f,		0.f,		vec3.m_z,	0.f,
				 0.f,		0.f,		0.f,		1.f };
	}

	Mat4 Mat4::CreateXRotationMatrix(float angle)
	{
		angle *= (float)(M_PI / 180);
		return { 1.f,	0.f,			 0.f,			0.f,
				 0.f,	cosf(angle),	-sinf(angle),	0.f,
				 0.f,	sinf(angle),	 cosf(angle),	0.f,
				 0.f,	0.f,			 0.f,			1.f };
	}

	Mat4 Mat4::CreateYRotationMatrix(float angle)
	{
		angle *= (float)(M_PI / 180);
		return { cosf(angle),	0.f,	sinf(angle),	0.f,
				 0.f,			1.f,	0.f,			0.f,
				-sinf(angle),	0.f,	cosf(angle),	0.f,
				 0.f,			0.f,	0.f,			1.f };
	}

	Mat4 Mat4::CreateZRotationMatrix(float angle)
	{
		angle *= (float)(M_PI / 180);
		return { cosf(angle),	-sinf(angle),	0.f,	0.f,
				 sinf(angle),	 cosf(angle),	0.f,	0.f,
				 0.f,			 0.f,			1.f,	0.f,
				 0.f,			 0.f,			0.f,	1.f };
	}

	Mat4 Mat4::CreatePerspectiveMatrix(float ar, float n, float f, float fov)
	{
		fov *= (float)(M_PI / 180);
		float t = tanf(fov / 2.f) * n;
		float r = t * ar;

		return { n / r,		0.f,		0.f,						0.f,
				0.f,		n / t,		0.f,						0.f,
				0.f,		0.f,		-(f + n) / (f - n),			-(2 * f * n) / (f - n),
				0.f,		0.f,		-1.f,						0.f };
	}

	Mat4 Mat4::CreateOrthographicMatrix(float w, float h, float d)
	{
		return { 2 / w,		0.f,		0.f,		0.f,
				 0.f,		2 / h,		0.f,		0.f,
				 0.f,		0.f,		-2 / d,		0.f,
				 0.f,		0.f,		0.f,		1.f };
	}

	Mat4 Mat4::CreateOrthographicMatrix(float r, float l, float t, float b, float n, float f)
	{
		return { 2 / (r - l),		 0.f,				 0.f,				 -(r + l) / (r - l),
				 0.f,				 2 / (t - b),		 0.f,				 -(t + b) / (t - b),
				 0.f,				 0.f,				-2 / (f - n),		 -(f + n) / (f - n),
				 0.f,				 0.f,				0.f,					1.f };
	}

	Mat4 Mat4::GetInverseMatrix() const
	{
		Mat4 inverse;
		float multiplier = 1 / GetDeterminant();
		float negativeMultiplier = 0 - multiplier;

		for (int row = 0; row < 4; row++)
			for (int col = 0; col < 4; col++)
			{
				inverse[col][row] = GetMinorDeterminant(row, col) *
					((row + col) % 2 == 0 ? multiplier : negativeMultiplier);
			}
		return inverse;
	}

	float Mat4::GetDeterminant() const
	{
		return (*this)[0][0] * GetMinorDeterminant(0, 0)
			- (*this)[0][1] * GetMinorDeterminant(0, 1)
			+ (*this)[0][2] * GetMinorDeterminant(0, 2)
			- (*this)[0][3] * GetMinorDeterminant(0, 3);
	}

	float Mat4::GetMinorDeterminant(int ignoredRow, int ignoredCol) const
	{
		int selectedRow[3];
		int selectedCol[3];
		for (int row = 0, i = 0, col = 0; i < 3; row++, i++, col++)
		{
			if (row == ignoredRow)
			{
				row++;
			}
			if (col == ignoredCol)
			{
				col++;
			}
			selectedRow[i] = row;
			selectedCol[i] = col;
		}
		return (*this)[selectedRow[0]][selectedCol[0]] * 
							GetMinorDeterminant(selectedRow[1],
												selectedCol[1],
												selectedRow[2],
												selectedCol[2])
			- (*this)[selectedRow[0]][selectedCol[1]] * 
							GetMinorDeterminant(selectedRow[1],
												selectedCol[0],
												selectedRow[2],
												selectedCol[2])
			+ (*this)[selectedRow[0]][selectedCol[2]] * 
							GetMinorDeterminant(selectedRow[1],
												selectedCol[0],
												selectedRow[2],
												selectedCol[1]);
	}

	float Mat4::GetMinorDeterminant(int selectedRow1, int selectedCol1, int selectedRow2, int selectedCol2) const
	{
		return (*this)[selectedRow1][selectedCol1] * (*this)[selectedRow2][selectedCol2]
			 - (*this)[selectedRow2][selectedCol1] * (*this)[selectedRow1][selectedCol2];
	}

	Mat4 Mat4::GetAdjoint() const
	{
		Mat4 adjoint;

		for (int row = 0; row < 3; row++)
			for (int col = 0; col < 3; col++)
			{
				adjoint[col][row] = GetMinorDeterminant(row, col) *
												((row + col) % 2 == 0 ? 1 : -1);
			}
		return adjoint;
	}

	Mat4 Mat4::GetTranspose() const
	{
		return { (*this)[0][0],	(*this)[1][0],	(*this)[2][0],	(*this)[3][0],
				 (*this)[0][1],	(*this)[1][1],	(*this)[2][1],	(*this)[3][1],
				 (*this)[0][2],	(*this)[1][2],	(*this)[2][2],	(*this)[3][2],
				 (*this)[0][3],	(*this)[1][3],	(*this)[2][3],	(*this)[3][3] };
	}

	Mat4& Mat4::operator*=(Mat4 const& other)
	{
		Mat4 product;
		for (size_t rowIndex = 0; rowIndex < 4; rowIndex++)
		{
			for (size_t columnIndex = 0; columnIndex < 4; columnIndex++)
			{
				for (size_t a = 0; a < 4; a++)
				{
					product[rowIndex][columnIndex] += (*this)[rowIndex][a] * other[a][columnIndex];
				}
			}
		}
		*this = product;
		return (*this);
	}

	Mat4 Mat4::operator*(Mat4 const& other) const
	{
		Mat4 product;
		for (size_t rowIndex = 0; rowIndex < 4; rowIndex++)
		{
			for (size_t columnnIndex = 0; columnnIndex < 4; columnnIndex++)
			{
				for (size_t a = 0; a < 4; a++)
				{
					product[rowIndex][columnnIndex] += (*this)[rowIndex][a] * other[a][columnnIndex];
				}
			}
		}
		return product;
	}

	Vec3 Mat4::operator*(Vec3 const& vec3) const
	{
		Vec3 product;
		float homogenizer = 0.f;

		for (size_t RowIndex = 0; RowIndex < 3; RowIndex++)
		{
			for (size_t column = 0; column < 3; column++)
			{
				product[RowIndex] += vec3[column] * (*this)[RowIndex][column];
			}
			product[RowIndex] += (*this)[RowIndex][3];
			homogenizer += (*this)[3][RowIndex] * vec3[RowIndex];
		}
		homogenizer += (*this)[3][3];


		if (homogenizer > 0.f)
		{
			product *= 1.f / homogenizer;	
		}

	

		return product;
	}

	Mat4::row Mat4::operator[](size_t rowIndex)
	{
#ifdef _DEBUG
		assert(rowIndex < 4);
#endif // _DEBUG

		return row(&m_matrix[rowIndex * 4]);
	}

	Mat4::const_row Mat4::operator[](size_t rowIndex) const
	{
#ifdef _DEBUG
		assert(rowIndex < 4);
#endif // _DEBUG

		return const_row(&m_matrix[rowIndex * 4]);
	}

	float& Mat4::row::operator[](size_t columnIndex)
	{
#ifdef _DEBUG
		assert(columnIndex < 4);
#endif // _DEBUG

		return m_ptr[columnIndex];
	}

	float Mat4::const_row::operator[](size_t columnIndex) const
	{
#ifdef _DEBUG
		assert(columnIndex < 4);
#endif // _DEBUG

		return m_ptr[columnIndex];
	}
}