#include "GameObjectManager.hpp"
#include "Engine.hpp"
#include "ArchManager.hpp"

namespace DarkDescent
{
	void GameObjectManager::onInitialize()
	{
		archManager_ = engine_.getSubSystem<ArchManager>();
	}

	void GameObjectManager::onReady()
	{

	}
	
	void GameObjectManager::onTerminate()
	{

	}

	GameObject* GameObjectManager::create(std::uint8_t archMapIndex)
	{
		Arch* arch = archManager_->rootArch(archMapIndex);
		Entity entity = arch->alloc();
		GameObjectHandle* handle = arch->getGameObjectHandle(entity);
		*handle = gameObjects_.emplace(arch, std::move(entity));
		logger.debug("created gameobject with ", *arch, " at buffer[", entity.bufferIndex ,"][", entity.index, "]");
		return handle->data;
	}
}