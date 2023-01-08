#include "GameObjectManager.hpp"
#include "Engine.hpp"
#include "ArchManager.hpp"
#include "js/GameObject.hpp"
#include "ScriptManager.hpp"

namespace DarkDescent
{
	void GameObjectManager::onInitialize()
	{
		archManager_ = engine_.getSubSystem<ArchManager>();

		addEventHandler(ScriptManager::Events::ENV_CREATED, [](SubSystem* self, const Event& e, void* data)
		{
			const JS::Env& env = *static_cast<const JS::Env*>(e.data);
			env.registerClass<JS::GameObjectClass>();
		}, this);
	}

	void GameObjectManager::onReady()
	{

	}
	
	void GameObjectManager::onTerminate()
	{

	}
	
	void GameObjectManager::reset(std::uint8_t archMapIndex)
	{
		assert(archMapIndex <= 1);
		gameObjectsAllocators_.at(archMapIndex).reset();
	}

	GameObject* GameObjectManager::create(std::uint8_t archMapIndex)
	{
		logger.debug("Create GameObject at mapIndex ", archMapIndex);
		assert(archMapIndex <= 1);
		Arch* arch = archManager_->rootArch(archMapIndex);
		Entity entity = arch->alloc();
		GameObjectHandle* handle = arch->getGameObjectHandle(entity);
		*handle = gameObjectsAllocators_.at(archMapIndex).emplace(arch, std::move(entity));
		logger.debug("created gameobject with ", *arch, " at buffer[", entity.bufferIndex ,"][", entity.index, "]");
		return handle->data;
	}
}