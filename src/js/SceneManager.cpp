#include "js/Env.hpp"
#include "js/SceneManager.hpp"
#include "SceneManager.hpp"
#include "js/Helpers.hpp"

namespace DarkDescent::JS::SceneManager
{
	JS_CALLBACK(registerScene)
	{

	}

	JS_CALLBACK(loadScene)
	{

	}

	JS_CALLBACK(getActiveScene)
	{

	}

	v8::Local<v8::Object> create(const Env& env, DarkDescent::SceneManager* system)
	{
		JS::Object obj(env);
		obj.set("registerScene", registerScene, system, v8::PropertyAttribute::ReadOnly);
		obj.set("registerScene", registerScene, system, v8::PropertyAttribute::ReadOnly);
		obj.set("registerScene", registerScene, system, v8::PropertyAttribute::ReadOnly);
		return *obj;
	}
};
