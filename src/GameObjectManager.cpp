#include "GameObjectManager.hpp"
#include "ArchManager.hpp"
#include "GameObject.hpp"
#include "Entity.hpp"
#include "Arch.hpp"
#include "ComponentManager.hpp"

namespace DarkDescent
{
	GameObjectManager::GameObjectManager(ComponentManager& componentManager, ArchManager& archManager):
		archManager_(archManager)
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
		GameObjectHandle* handle = arch.getGameObject(entity);
		if(!handle->hasValue())
			*handle = gameObjects_.emplace(arch, std::forward<Entity>(entity));
		return *(handle->data);
	}
}