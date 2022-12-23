#include "GameObjectManager.hpp"
#include "Engine.hpp"
#include "ArchManager.hpp"

namespace DarkDescent
{
	void GameObjectManager::onInitialize()
	{
		archManager_ = engine_.getSubSystem<ArchManager>();
	}
	
	void GameObjectManager::onTerminate()
	{

	}

	GameObject* GameObjectManager::create()
	{
		Arch* arch = archManager_->rootArch();
		Entity entity = arch->alloc();
		GameObjectHandle* handle = arch->getGameObjectHandle(entity);
		*handle = gameObjects_.emplace(arch, std::move(entity));
		return handle->data;
	}
}