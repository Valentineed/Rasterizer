#pragma once

#include "Vec3.h"

namespace Rasterizer
{
	class TransformMatrix
	{
	public:

		TransformMatrix() = default;
		TransformMatrix(float a, float b, float c, float d,
						float e, float f, float g, float h,
						float i, float j, float k, float l);
		~TransformMatrix() = default;

		static TransformMatrix CreateIdentityMatrix();

		static TransformMatrix CreateTransformMatrix(const Vec3 & position, const Vec3 & rotation, const Vec3 & scale);

		static TransformMatrix CreateTranslationMatrix(const Vec3 & vec3);
		static TransformMatrix CreateScaleMatrix(const Vec3 & vec3);
		static TransformMatrix CreateRotationMatrix(const Vec3 & vec3);
		static TransformMatrix CreateXRotationMatrix(float angle);
		static TransformMatrix CreateYRotationMatrix(float angle);
		static TransformMatrix CreateZRotationMatrix(float angle);

		TransformMatrix ExtractRotation();

		TransformMatrix& operator*=(TransformMatrix const& other);
		TransformMatrix operator*(TransformMatrix const& other) const;
		Vec3 operator*(Vec3 const& vec3) const;

	private:
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

		row operator[](size_t index);
		const_row operator[](size_t index) const;

		float m_matrix[12] = { 0.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 0.f, 0.f };
	};
}