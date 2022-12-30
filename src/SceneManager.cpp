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

	void SceneManager::loadScene(const char* name, const JS::Env& env)
	{
		const Hash hash = Hasher::hash(name);
		if (!scenes_.contains(hash))
		{
			throw TraceException("Could not find scene!");
		}

		Scene* scene = std::addressof(scenes_.at(hash));

		if (loadingScene_.has_value())
		{
			env.throwException("Cannot load a scene while another scene is loading!");
			return;
		}

		Scene* oldScene = activeScene_.value_or(nullptr);

		if (oldScene == scene)
		{
			env.throwException("cannot load a scene that is already loaded!");
			return;
		}
		else
		{
			loadingScene_ = scene;
			scene->onLoad(archManager_.getNextActiveIndex());
			activeScene_ = scene;
			loadingScene_.reset();
			archManager_.swapActiveIndex();

			if (oldScene != nullptr)
			{
				const std::uint8_t index = oldScene->archMapIndex();
				oldScene->onUnload();
				gameObjectManager_.reset(index);
			}
		}
	}
}