#include "js/Env.hpp"
#include "js/SceneManager.hpp"
#include "SceneManager.hpp"
#include "js/Helpers.hpp"
#include "Scene.hpp"
#include "Engine.hpp"

namespace DarkDescent::JS::SceneManager
{
	DarkDescent::SceneManager& getSceneManager(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Local<v8::Value> data = args.Data();
		assert(data->IsExternal());
		return *static_cast<DarkDescent::SceneManager*>(data.As<v8::External>()->Value());
	}

	JS_CALLBACK(registerScene)
	{
		auto& sm = getSceneManager(args);
		const int l = args.Length();

		if (l < 2)
		{
			env.throwException("Not enough arguments!");
		}
		else if (!args[0]->IsString())
		{
			env.throwException("First argument is not a string!");
		}
		else if (!args[1]->IsFunction())
		{
			env.throwException("Second argument is not a class!");
		}
		else
		{
			const std::string name = JS::parseString(env, args[0]);

			v8::Local<v8::Value> ctorArgs[1] = { args[0] };

			v8::Local<v8::Value> scene;

			if (!args[1].As<v8::Function>()->CallAsConstructor(env.context(), 1, ctorArgs).ToLocal(&scene))
			{
				env.throwException("Could not call type as Scene constructor!");
			}
			else
			{
				sm.registerScene(name.c_str(), env, scene);
			}
		}
	}

	JS_CALLBACK(loadScene)
	{
		auto& sm = getSceneManager(args);
		const int l = args.Length();
		if (l < 1)
		{
			env.throwException("Not enough arguments!");
		}
		else if (!args[0]->IsString())
		{
			env.throwException("First argument is not a string!");
		}
		else
		{
			const std::string name = JS::parseString(env, args[0]);
			sm.loadScene(name.c_str(), env);
		}
	}

	JS_CALLBACK(getActiveScene)
	{
		auto& sm = getSceneManager(args);
		auto scene = sm.activeScene();
		if (scene.has_value())
		{
			args.GetReturnValue().Set(scene.value()->jsScene().value());
		}
		else
		{
			args.GetReturnValue().SetNull();
		}
	}

	v8::Local<v8::Object> create(const Env& env)
	{
		JS::Object obj(env);
		DarkDescent::SceneManager* system = Engine::getInstance().getSubSystem<DarkDescent::SceneManager>();
		obj.set("registerScene", registerScene, system, v8::PropertyAttribute::ReadOnly);
		obj.set("loadScene", loadScene, system, v8::PropertyAttribute::ReadOnly);
		obj.set("getActiveScene", getActiveScene, system, v8::PropertyAttribute::ReadOnly);
		return *obj;
	}
};
