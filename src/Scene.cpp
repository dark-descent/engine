#include "Scene.hpp"
#include "Resource.hpp"

namespace DarkDescent
{
	Scene::Scene(const std::string& name, const JS::Env& env, v8::Local<v8::Value> jsScene):
		name(name),
		jsScene_(env)
	{
		jsScene_.reset(jsScene);
	}

	Scene::~Scene() { }

	void Scene::onLoad()
	{
		jsScene_.onLoad();
	}

	void Scene::onUnload()
	{
		jsScene_.onUnload();
	}
}