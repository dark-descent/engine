#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "ResourceManager.hpp"
#include "ScriptManager.hpp"
#include "js/Scene.hpp"
#include "js/SceneManager.hpp"

namespace DarkDescent
{
	void SceneManager::onInitialize()
	{
		addEventHandler(ScriptManager::Events::ENV_CREATED, EVENT_HANDLER()
		{
			assert(data);
			const JS::Env& env = *static_cast<const JS::Env*>(event.data);
			env.global().set("Scene", env.registerClass<JS::SceneClass>());
			env.engineNamespace().set("SceneManager", JS::SceneManager::create(env, static_cast<SceneManager*>(data)));
		}, this);
	}

	void SceneManager::onReady()
	{

	}

	void SceneManager::onTerminate()
	{

	}

	void SceneManager::registerScene(const char* path)
	{
		std::string p(path);
		resourceManager_.registerResource<Scene>(p, p);
	}

	Scene& SceneManager::loadScene(const char* path)
	{
		return resourceManager_.getResource<Scene>(path);
	}
}