#include "pch.h"
#include "Entity.h"

namespace Rasterizer
{
	Entity::Entity(Mesh* mesh) :
		m_mesh(mesh)
	{
	}

	Entity::Entity(Mesh* mesh, float alpha) :
		m_mesh(mesh),
		m_alpha(alpha)
	{
	}

	Entity::Entity(Mesh* mesh, Vec3 position, Vec3 rotation, Vec3 scale, float alpha) :
		m_mesh(mesh),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale),
		m_alpha(alpha)
	{
	}

	Mat4 Entity::GetTransformation() const
	{
		return Mat4::CreateTransformMatrix(m_position, m_rotation, m_scale);
	}

	void Entity::ResetTransformation()
	{
		m_position = {};
		m_rotation = {};
		m_scale = { 1.f, 1.f, 1.f };
	}

	void Entity::Translate(Vec3 movement)
	{
		m_position += movement;
	}

	void Entity::Scale(Vec3 scale)
	{
		m_scale += scale;
	}

	void Entity::Rotate(Vec3 rotation)
	{
		m_rotation += rotation;
	}

	Mat4 Entity::GetNormalTransformation() const
	{
		Mat4 matrix = Mat4::CreateScaleMatrix({ 1 / m_scale.m_x, 1 / m_scale.m_y, 1 / m_scale.m_z });
		matrix *= Mat4::CreateXRotationMatrix(-m_rotation.m_x);
		matrix *= Mat4::CreateZRotationMatrix(-m_rotation.m_z);
		matrix *= Mat4::CreateYRotationMatrix(-m_rotation.m_y);
		return matrix.GetTranspose();
	}

	std::vector<Vec3> const& Entity::GetNormalBuffer() const
	{
		return m_mesh->GetNormalBuffer();
	}

	std::vector<Vec3> const& Entity::GetCoordinateBuffer() const
	{
		return m_mesh->GetCoordinateBuffer();
	}

	float Entity::GetAlpha() const
	{
		return m_alpha;
	}

	void Entity::SetAlpha(float alpha)
	{
		m_alpha = alpha;
	}

	std::vector<Vec2> const& Entity::GetTexelBuffer() const
	{
		return m_mesh->GetTexelBuffer();
	}

	std::vector<Triangle>::const_iterator Entity::begin() const
	{
		return m_mesh->GetTriangleBuffer().begin();
	}

	std::vector<Triangle>::const_iterator Entity::end() const
	{
		return m_mesh->GetTriangleBuffer().end();
	}
}