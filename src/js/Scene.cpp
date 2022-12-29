#include "pch.hpp"
#include "js/Scene.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"
#include "SceneManager.hpp"

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

	JS_CREATE_CLASS(SceneClass)
	{
		builder.setName("Scene");
		builder.setConstructor(ctor);
		builder.setMethod("onLoad");
		builder.setMethod("onReady");
		builder.setMethod("onUnload");
		builder.setInternalFieldCount(1);
	}
}