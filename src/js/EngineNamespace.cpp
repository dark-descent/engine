#include "js/EngineNamespace.hpp"
#include "js/Env.hpp"
#include "js/Object.hpp"
#include "js/Helpers.hpp"
#include "Engine.hpp"
#include "WindowManager.hpp"

namespace DarkDescent::JS::Engine
{
	JS_CALLBACK(initialize)
	{
		if (args.Length() < 1)
		{
			env.throwException("Not enough arguments!");
		}
		else if (!args[0]->IsFunction())
		{
			env.throwException("First argument is not a class!");
		}
		else
		{
			auto engine = JS::parseExternalData<DarkDescent::Engine>(args);

			v8::Local<v8::Value> engineCtorArgs[1] = {
				v8::External::New(env.isolate(), engine)
			};

			v8::Local<v8::Value> gameVal;
			if (!args[0].As<v8::Function>()->CallAsConstructor(env.context(), 1, engineCtorArgs).ToLocal(&gameVal)) [[unlikely]]
			{
				env.throwException("Could not construct Game class!");
			}
			else [[likely]]
			{
				engine->initializeGame(env, gameVal.As<v8::Object>(), args);
				args.GetReturnValue().Set(gameVal);
			}
		}
	}

	v8::Local<v8::Object> createNamespace(const JS::Env& env)
	{
		auto enginePtr = std::addressof(DarkDescent::Engine::getInstance());
		JS::Object engine(env);
		engine.set("initialize", initialize, enginePtr, v8::PropertyAttribute::ReadOnly);
		return *engine;
	}

}