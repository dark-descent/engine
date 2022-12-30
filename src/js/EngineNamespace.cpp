#include "js/EngineNamespace.hpp"
#include "js/Env.hpp"
#include "js/Object.hpp"
#include "js/Helpers.hpp"
#include "Engine.hpp"

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
			auto engine = JS::parseExternal<DarkDescent::Engine>(args);
			v8::Local<v8::Value> engineVal;
			if (!args[0].As<v8::Function>()->CallAsConstructor(env.context(), 0, nullptr).ToLocal(&engineVal))
			{
				env.throwException("Could not construct Game class!");
			}
			else
			{
				engine->initializeGame(env, engineVal.As<v8::Object>());
				
				if (args.Length() == 2)
				{
					engine->game()->onInitialize({ args[1] });
				}
				else
				{
					engine->game()->onInitialize();
				}

				args.GetReturnValue().Set(engineVal);
			}
		}
	}

	v8::Local<v8::Object> createNamespace(const JS::Env& env)
	{
		auto enginePtr = std::addressof(DarkDescent::Engine::getInstance());
		JS::Object engine(env);
		engine.set("initialize", initialize, enginePtr, v8::PropertyAttribute::ReadOnly);
		engine.set("test", JS::string(env, "test"), v8::PropertyAttribute::ReadOnly);
		return *engine;
	}

}