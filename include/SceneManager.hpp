#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"
#include "Hash.hpp"
#include "Engine.hpp"
#include "ArchManager.hpp"
#include "GameObjectManager.hpp"
#include "Scene.hpp"

namespace DarkDescent
{
	class Scene;

	class SceneManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(SceneManager),
			archManager_(*engine_.getSubSystem<ArchManager>()),
			gameObjectManager_(*engine.getSubSystem<GameObjectManager>())
		{ };

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	public:
		Scene& registerScene(const char* name, const JS::Env& env, v8::Local<v8::Value> scene);
		void loadScene(const char* name, const JS::Env& env);
		const std::optional<Scene*>& activeScene() const { return activeScene_; }
		const std::optional<Scene*>& loadingScene() const { return loadingScene_; }

	private:
		ArchManager& archManager_;
		GameObjectManager& gameObjectManager_;
		
		std::optional<Scene*> loadingScene_;
		std::optional<Scene*> activeScene_;
		std::unordered_map<Hash, Scene> scenes_;
	};
}