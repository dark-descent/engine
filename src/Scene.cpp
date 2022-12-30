#include "Scene.hpp"
#include "Resource.hpp"
#include "ArchManager.hpp"
#include "Engine.hpp"
#include "GameObjectManager.hpp"

namespace DarkDescent
{
	Scene::Scene(const std::string& name, const JS::Env& env, v8::Local<v8::Value> jsScene):
		name(name),
		archManager_(*Engine::getInstance().getSubSystem<ArchManager>()),
		gameObjectManager_(*Engine::getInstance().getSubSystem<GameObjectManager>()),
		jsScene_(env),
		archMapIndex_(ArchManager::UNSET_INDEX)
	{
		jsScene.As<v8::Object>()->SetInternalField(0, v8::External::New(env.isolate(), this));
		jsScene_.reset(jsScene);
	}

	Scene::~Scene() { }

	void Scene::onLoad(std::uint8_t archMapIndex)
	{
		Logger::get().debug("Scene::onLoad() -> ", archMapIndex);
		archMapIndex_ = archMapIndex;
		jsScene_.onLoad();
	}

	void Scene::onUnload()
	{
		archMapIndex_ = ArchManager::UNSET_INDEX;
		jsScene_.onUnload();
	}

	void Scene::spawn()
	{
		gameObjectManager_.create(archMapIndex_);
	}
}