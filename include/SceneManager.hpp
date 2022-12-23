#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"
#include "Hash.hpp"

namespace DarkDescent
{
	class Scene;

	class SceneManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(SceneManager) {};

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;

	public:
		Scene& loadScene(const char* path);

	private:
		std::optional<Scene*> activeScene_;
		std::optional<Scene*> loadingScene_;

		std::unordered_map<Hash, Scene> scenes_;
	};
}