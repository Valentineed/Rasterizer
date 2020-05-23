#pragma once
#include <vector>
#include "Entity.h"
#include "Light.h"
namespace Rasterizer
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		Entity& AddEntity(Entity const& entity);
		Entity& AddEntity(Entity&& entity);

		Light& AddLight(Light const& light);
		Light& AddLight(Light&& light);

		std::vector<Light> GetLight() const;

		const std::vector<Entity>::const_iterator begin() const;
		const std::vector<Entity>::iterator begin();

		const std::vector<Entity>::const_iterator end() const;

	private:
		std::vector<Entity> m_entities;
		std::vector<Light> m_lights;
	};
}