#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "ResourceManager.hpp"
#include "ScriptManager.hpp"
#include "js/Scene.hpp"
#include "js/SceneManager.hpp"
#include "Logger.hpp"

namespace DarkDescent
{
	void SceneManager::onInitialize()
	{
		addEventHandler(ScriptManager::Events::ENV_CREATED, EVENT_HANDLER()
		{
			const JS::Env& env = *static_cast<const JS::Env*>(event.data);
			env.registerClass<JS::SceneClass>();
		});
	}

	void SceneManager::onReady()
	{

	}

	void SceneManager::onTerminate()
	{
		scenes_.clear();
	}

	Scene& SceneManager::registerScene(const char* name, const JS::Env& env, v8::Local<v8::Value> scene)
	{
		logger.debug("Registering scene ", name);

		const Hash hash = Hasher::hash(name);
		if (!scenes_.contains(hash))
		{
			scenes_.emplace(std::piecewise_construct, std::forward_as_tuple(hash), std::forward_as_tuple(std::string(name), env, scene));
		}
		return scenes_.at(hash);
	}

	Scene& SceneManager::loadScene(const char* name, const JS::Env& env)
	{
		logger.info("Loading scene ", name);

		const Hash hash = Hasher::hash(name);
		if(!scenes_.contains(hash))
		{
			throw TraceException("Could not find scene!");
		}

		Scene& scene = scenes_.at(hash);
		scene.onLoad(archManager_.getNextActiveIndex());

		Scene* oldScene = activeScene_.has_value() ? activeScene_.value() : nullptr;
		
		activeScene_ = std::addressof(scene);
		archManager_.swapActiveIndex();
		
		if(oldScene != nullptr)
			oldScene->onUnload();
		
		return scene;
	}
}