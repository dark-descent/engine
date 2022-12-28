#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "ResourceManager.hpp"

namespace DarkDescent
{
	void SceneManager::onInitialize()
	{

	}

	void SceneManager::onReady()
	{

	}

	void SceneManager::onTerminate()
	{

	}

	Scene& SceneManager::loadScene(const char* path)
	{
		ResourceManager* rm = engine_.getSubSystem<ResourceManager>();

		Hash hash = Hasher::hash(path);
		if (!scenes_.contains(hash))
			scenes_.emplace(hash, Scene(path, rm->getResource(path)));
		Scene& scene = scenes_.at(hash);
		scene.load();
		if(activeScene_.has_value())
			activeScene_.value()->unload();
		activeScene_.emplace(std::addressof(scene));
		return scene;
	}
}