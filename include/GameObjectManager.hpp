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

		GameObject* create(std::uint8_t archMapIndex);
		void reset(std::uint8_t archMapIndex);

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	private:
		ArchManager* archManager_;
		std::array<PersistentAllocator<GameObject>, 2> gameObjectsAllocators_;
	};
}