#include "js/EngineModule.hpp"
#include "js/Env.hpp"
#include "js/Helpers.hpp"
#include "js/Scene.hpp"
#include "js/SceneManager.hpp"
#include "js/Window.hpp"
#include "js/Game.hpp"
#include "Logger.hpp"
#include "js/EngineNamespace.hpp"

namespace DarkDescent::JS::EngineModule
{
	static const char* importName = "engine";

	bool isImportString(const std::string& str)
	{
		return str.compare(importName) == 0;
	}

	v8::Local<v8::Module> create(const Env& env)
	{
		std::vector<v8::Local<v8::String>> exports = {
			JS::string(env, "default"),
			JS::string(env, "Scene"),
			JS::string(env, "SceneManager"),
			JS::string(env, "Game"),
			JS::string(env, "Window"),
		};

		v8::Local<v8::Module> module = v8::Module::CreateSyntheticModule(env.isolate(), JS::string(env, importName), exports, [](v8::Local<v8::Context> context, v8::Local<v8::Module> module) -> v8::MaybeLocal<v8::Value>
		{
			const Env& env = Env::fromContext(context);

		auto isolate = context->GetIsolate();

		auto exportValue = [ & ](const char* name, v8::Local<v8::Value> val)
		{
			module->SetSyntheticModuleExport(env.isolate(), JS::string(env, name), val);
		};

		exportValue("Scene", env.getJSClass<SceneClass>());
		exportValue("Game", env.getJSClass<GameClass>());
		exportValue("Window", env.getJSClass<WindowClass>());
		exportValue("SceneManager", SceneManager::create(env));

		if (module->SetSyntheticModuleExport(env.isolate(), JS::string(env, "default"), JS::Engine::createNamespace(env)).IsNothing())
		{
			Logger::get().error("Could not create default export for module \"", importName, "\"!");
			return v8::MaybeLocal<v8::Value>(False(isolate));
		}

		return v8::MaybeLocal<v8::Value>(v8::True(isolate));
		});

		v8::Maybe<bool> result = module->InstantiateModule(env.context(), ModuleLoader::importModule);

		if (result.IsNothing())
		{
			throw std::runtime_error("Can't instantiate module!");
		}
		else if (!result.FromJust())
		{
			throw std::runtime_error("Module instantiation failed!");
		}
		else
		{
			return module;
		}
	}
}