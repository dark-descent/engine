#include "pch.hpp"
#include "js/Scene.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"
#include "SceneManager.hpp"
#include "Logger.hpp"
#include "GameObject.hpp"
#include "js/GameObject.hpp"

namespace DarkDescent::JS
{
	Scene::Scene(const Env& env) : ObjectWrapper(env) { }

	Scene::~Scene() { }

	void Scene::initializeProps()
	{
		loadMethod(onLoad_, "onLoad");
		loadMethod(onReady_, "onReady");
		loadMethod(onUnload_, "onUnload");
	}

	JS_METHOD_IMPL(Scene::onLoad);
	JS_METHOD_IMPL(Scene::onReady);
	JS_METHOD_IMPL(Scene::onUnload);

	JS_CLASS_METHOD_IMPL(SceneClass::ctor)
	{
		JS::Object self(env, args.This());
		self.set("name", args[0], v8::PropertyAttribute::ReadOnly);
	}

	JS_CLASS_METHOD_IMPL(SceneClass::onLoad)
	{
		
	}

	JS_CLASS_METHOD_IMPL(SceneClass::onReady)
	{
		
	}

	JS_CLASS_METHOD_IMPL(SceneClass::onUnload)
	{
		
	}

	JS_CLASS_METHOD_IMPL(SceneClass::spawn)
	{
		DarkDescent::Scene* scene = JS::getInternalPointer<DarkDescent::Scene>(args);
		DarkDescent::GameObject* obj = scene->spawn();
		auto jsObj = env.getClass<JS::GameObjectClass>().instantiate({ v8::External::New(env.isolate(), obj) }).ToLocalChecked();
		args.GetReturnValue().Set(jsObj);
	}

	JS_CREATE_CLASS(SceneClass)
	{
		builder.setName("Scene");
		builder.setConstructor(ctor);
		builder.setMethod("onLoad");
		builder.setMethod("onReady");
		builder.setMethod("onUnload");
		builder.setMethod("spawn", spawn);
		builder.setInternalFieldCount(1);
	}
}