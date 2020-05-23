#pragma once
#include "Mesh.h"
#include "Mat4.h"
#include "Triangle.h"

namespace Rasterizer
{
	class Entity
	{
	public:
		Entity() = delete;
		Entity(Mesh* mesh);
		Entity(Mesh* mesh, float alpha);
		Entity(Mesh* mesh, Vec3 position, Vec3 rotation = {}, Vec3 scale = { 1.f, 1.f, 1.f }, float alpha = 1.f);
		~Entity() = default;

		Mat4 GetTransformation() const;
		void ResetTransformation();

		void Translate(Vec3 movement);
		void Scale(Vec3 scale);
		void Rotate(Vec3 rotation);


		Mat4 GetNormalTransformation() const;
		std::vector<Vec3> const& GetNormalBuffer() const;
		std::vector<Vec3> const& GetCoordinateBuffer() const;
		std::vector<Vec2> const& GetTexelBuffer() const;

		float GetAlpha() const;
		void SetAlpha(float alpha);

		std::vector<Triangle>::const_iterator begin() const;
		std::vector<Triangle>::const_iterator end() const;

	private:
		Mesh const* m_mesh;

		float m_alpha = 1.f;
		Vec3 m_position = {};
		Vec3 m_rotation = {};
		Vec3 m_scale = { 1.f, 1.f, 1.f };
	};
}