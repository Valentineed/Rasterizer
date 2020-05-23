#include "pch.h"
#include "Scene.h"

namespace Rasterizer
{
	Scene::~Scene()
	{
		for (size_t i = 0; i < m_entities.size(); i++)
		{
			m_entities[i].~Entity();
		}
	}

	Entity& Scene::AddEntity(Entity const& entity)
	{
		m_entities.push_back(entity);
		return m_entities.back();
	}

	Entity& Scene::AddEntity(Entity&& entity)
	{
		m_entities.push_back(std::forward<Entity>(entity));
		return m_entities.back();
	}

	Light& Scene::AddLight(Light const& light)
	{
		m_lights.push_back(light);
		return m_lights.back();
	}

	Light& Scene::AddLight(Light&& light)
	{
		m_lights.push_back(std::forward<Light>(light));
		return m_lights.back();
	}

	std::vector<Light> Scene::GetLight() const
	{
		return m_lights;
	}

	const std::vector<Entity>::const_iterator Scene::begin() const
	{
		return m_entities.begin();
	}

	const std::vector<Entity>::iterator Scene::begin()
	{
		return m_entities.begin();
	}

	const std::vector<Entity>::const_iterator Scene::end() const
	{
		return m_entities.end();
	}
}