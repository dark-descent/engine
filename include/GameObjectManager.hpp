#pragma once

#include "SubSystem.hpp"
#include "GameObject.hpp"
#include "PersistentAllocator.hpp"

namespace DarkDescent
{
	class ArchManager;

	class GameObjectManager final: public SubSystem
	{	
	public:
		SUB_SYSTEM_CTORS(GameObjectManager), archManager_(nullptr) {}

		GameObject* create();

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	private:
		ArchManager* archManager_;
		PersistentAllocator<GameObject> gameObjects_;
	};
}