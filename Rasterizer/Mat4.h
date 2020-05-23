#pragma once

#include "Vec3.h"

namespace Rasterizer
{
	class Mat4
	{
	public:
		struct row
		{
			row(float* ptr) : m_ptr(ptr) {}
			float* m_ptr;

			float& operator[](size_t columIndex);
		};
		struct const_row
		{
			const_row(float const* ptr) : m_ptr(ptr) {}
			float const* m_ptr;

			float operator[](size_t columIndex) const;
		};

		Mat4() = default;
		Mat4(float a, float b, float c, float d,
			 float e, float f, float g, float h,
			 float i, float j, float k, float l,
			 float m, float n, float o, float p);
		~Mat4() = default;

		static Mat4 CreateIdentityMatrix();

		static Mat4 CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale);

		static Mat4 CreateScaleMatrix(const Vec3& vec3);
		static Mat4 CreateTranslationMatrix(const Vec3& vec3);
		static Mat4 CreateXRotationMatrix(float angle);
		static Mat4 CreateYRotationMatrix(float angle);
		static Mat4 CreateZRotationMatrix(float angle);

		static Mat4 CreatePerspectiveMatrix(float aspectRatio, float near, float far, float fov);
		static Mat4 CreateOrthographicMatrix(float width, float height, float depth);
		static Mat4 CreateOrthographicMatrix(float right, float left, float top, float bottom, float near, float far);

		Mat4 GetInverseMatrix() const;
		float GetDeterminant() const;
		float GetMinorDeterminant(int ignoredRow, int ignoredCol) const;
		float GetMinorDeterminant(int selectedRow1, int selectedCol1, int selectedRow2, int selectedCol2) const;
		Mat4 GetAdjoint() const;
		Mat4 GetTranspose() const;
		

		Mat4& operator*=(Mat4 const& other);
		Mat4 operator*(Mat4 const& other) const;
		Vec3 operator*(Vec3 const& vec3) const;
		row operator[](size_t index);
		const_row operator[](size_t index) const;

	private:

		float m_matrix[16] = {  0.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 0.f, 0.f };
	};
}