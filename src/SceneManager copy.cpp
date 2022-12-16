#include "SceneManager.hpp"
#include "Scene.hpp"

namespace DarkDescent
{
	void SceneManager::onInitialize()
	{

	}

	void SceneManager::onTerminate()
	{

	}

	Scene& SceneManager::loadScene(const char* path)
	{
		Hash hash = Hasher::hash(path);
		if (!scenes_.contains(hash))
			scenes_.emplace(hash, Scene(path));
		Scene& scene = scenes_.at(hash);
		scene.load();
		if(activeScene_.has_value())
			activeScene_.value().unload();
		activeScene_.emplace(scene);
		return scene;
	}
}