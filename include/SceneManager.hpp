#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"
#include "Hash.hpp"
#include "ResourceManager.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	class Scene;

	class SceneManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(SceneManager), 
			resourceManager_(*engine_.getSubSystem<ResourceManager>())
		{ };

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	public:
		void registerScene(const char* path);
		Scene& loadScene(const char* path);

	private:
		ResourceManager& resourceManager_;
		std::optional<Scene*> activeScene_;
		std::optional<Scene*> loadingScene_;
	};
}