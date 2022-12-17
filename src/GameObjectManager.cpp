#include "GameObjectManager.hpp"
#include "ArchManager.hpp"
#include "GameObject.hpp"
#include "Entity.hpp"
#include "Arch.hpp"
#include "ComponentManager.hpp"
#include "GameObjectComponent.hpp"

namespace DarkDescent
{
	GameObjectManager::GameObjectManager(ComponentManager& componentManager, ArchManager& archManager):
		archManager_(archManager),
		gameObjectComponent_(componentManager.registerComponent<GameObjectComponent>())
	{

	}

	GameObjectManager::~GameObjectManager()
	{

	}

	GameObject& GameObjectManager::create()
	{
		Arch& arch = archManager_.rootArch();
		return getFromEntity(arch, arch.allocEntity());
	}

	GameObject& GameObjectManager::getFromEntity(Arch& arch, Entity&& entity)
	{
		Arch& newArch = arch.addComponent(entity, gameObjectComponent_);
		GameObjectComponent* component = static_cast<GameObjectComponent*>(newArch.getComponent(entity, gameObjectComponent_));
		component->gameObject = gameObjects_.emplace(newArch, std::move(entity));
		return *component->gameObject.data;
	}
}