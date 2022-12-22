#pragma once

#include "GameObject.hpp"
#include "PersistentAllocator.hpp"
#include "ComponentInfo.hpp"

namespace DarkDescent
{
	struct Entity;
	class ArchManager;
	class Arch;
	class ComponentManager;

	class GameObjectManager
	{
	public:
		GameObjectManager(ComponentManager& componentManager, ArchManager& archManager);
		~GameObjectManager();

		GameObject& create();

		GameObject& getFromEntity(Arch& arch, Entity&& entity);

	private:
		ArchManager& archManager_;
		PersistentAllocator<GameObject> gameObjects_;
	};
}