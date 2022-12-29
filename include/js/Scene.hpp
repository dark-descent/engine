#pragma once

#include "js/Class.hpp"

namespace DarkDescent::JS
{
	class Env;

	class Scene: public ObjectWrapper
	{
	public:
		Scene(const Env& env);
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene();

		virtual void initializeProps();

		JS_METHOD_DECL(onLoad);
		JS_METHOD_DECL(onReady);
		JS_METHOD_DECL(onUnload);
	};

	class SceneClass: public Class
	{
		JS_CLASS_BODY(SceneClass);

	private:
		JS_CLASS_METHOD(ctor);
		JS_CLASS_METHOD(onLoad);
		JS_CLASS_METHOD(onReady);
		JS_CLASS_METHOD(onUnload);
	};
}