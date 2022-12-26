#include "js/ModuleLoader.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "js/Helpers.hpp"
#include "js/Console.hpp"

namespace DarkDescent::JS
{
	using namespace v8;

	ModuleLoader::ModuleLoader(const Env& env):
		env(env),
		checkPaths_(),
		isLoaded_(false)
	{ }

	ModuleLoader::~ModuleLoader()
	{

	}

	/*static*/ v8::MaybeLocal<v8::Module> ModuleLoader::importModule(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer)
	{
		const int id = referrer->ScriptId();

		Env& env = *static_cast<Env*>(context->GetIsolate()->GetData(0));

		std::string import = JS::parseString(env, specifier);

		std::replace(import.begin(), import.end(), '\\', '/');

		// if (import.compare("native-js") == 0)
		// 	return v8::MaybeLocal<v8::Module>(env.nativeJSModule_.Get(env.isolate()));

		std::filesystem::path importPath = import;
		std::filesystem::path fromPath;

		if (!importPath.is_relative())
		{
			fromPath = std::filesystem::path("");
		}
		else if (env.moduleLoader().modulesPaths_.contains(id))
		{
			fromPath = (std::filesystem::path(env.moduleLoader().modulesPaths_.at(id)) / "..").lexically_normal();
		}
		else
		{
			fromPath = Utils::getExecutableDir();
		}

		if (importPath.is_relative())
			importPath = fromPath / importPath;

		if (!std::filesystem::exists(importPath))
		{
			bool isValid = false;

			std::array<std::string, 3> resolves = {
				".ts",
				".tsx",
				"json"
			};
			
			for (const std::string& t : resolves)
			{
				std::filesystem::path checkPath = importPath;
				checkPath += t;
				if (std::filesystem::exists(checkPath))
				{
					importPath = checkPath;
					isValid = true;
					break;
				}
			}

			if (!isValid)
			{
				Logger::get().warn("Could not resolve import path for ", importPath.string().c_str(), "!");
				return v8::MaybeLocal<v8::Module>();
			}
		}

		const std::string ext = importPath.extension().string();

		v8::MaybeLocal<v8::Module> module;

		if (ext.compare(".json") == 0)
		{
			module = env.moduleLoader().loadJsonModule(importPath.string().c_str());
		}
		else
		{
			module = env.moduleLoader().loadModule(importPath.string().c_str());
		}

		return v8::MaybeLocal<v8::Module>(module);
	}

	v8::MaybeLocal<v8::Module> ModuleLoader::loadJsonModule(const char* filePath) const
	{
		using namespace v8;

		TryCatch tryCatcher(env.isolate());

		const std::string path(filePath);

		if (modules_.contains(path))
			return v8::MaybeLocal<v8::Module>();

		std::ifstream is(filePath);
		std::string jsonString((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

		std::vector<v8::Local<v8::String>> exports({ JS::string(env, "default") });

		Local<Module> module = Module::CreateSyntheticModule(env.isolate(), JS::string(env, filePath), exports, [](Local<Context> context, Local<Module> module)
		{
			Env& env = *static_cast<Env*>(context->GetIsolate()->GetData(0));
			module->SetSyntheticModuleExport(context->GetIsolate(), v8::String::NewFromUtf8(context->GetIsolate(), "default").ToLocalChecked(), env.moduleLoader().getJsonData(module->GetIdentityHash()).ToLocalChecked());
			return MaybeLocal<Value>(v8::True(env.isolate()));
		});

		Local<Value> json = v8::JSON::Parse(env.context(), JS::string(env, jsonString)).ToLocalChecked();

		jsonModules_.emplace(module->GetIdentityHash(), new Persistent<Value>(env.isolate(), json));
		modules_.emplace(path, new Persistent<Module>(env.isolate(), module));

		Maybe<bool> result = module->InstantiateModule(env.context(), ModuleLoader::importModule);

		if (result.IsNothing())
		{
			Logger::get().error("Can't instantiate module.");
			return v8::MaybeLocal<v8::Module>();
		}
		else
		{
			MaybeLocal<Value> result = module->Evaluate(env.context());
			return v8::MaybeLocal<v8::Module>(module);
		}
	}

	v8::MaybeLocal<v8::Value> ModuleLoader::getJsonData(const int moduleHash) const
	{
		if (jsonModules_.contains(moduleHash))
			return v8::MaybeLocal<v8::Value>(jsonModules_.at(moduleHash)->Get(env.isolate()));
		return v8::MaybeLocal<v8::Value>();
	}

	v8::MaybeLocal<v8::Module> ModuleLoader::loadModule(const char* filePath) const
	{
		using namespace v8;

		std::filesystem::path path(filePath);

		if (path.is_relative())
			path = (Engine::getInstance().gamePath / path).lexically_normal();

		std::string p = (path / "..").lexically_normal().string();
		std::replace(p.begin(), p.end(), '\\', '/');
		std::string fileName = path.filename().string();

		std::ifstream is(path);
		std::string code = std::string("const __dirname = \"") + p + "\"; const __filename = \"" + fileName + "\";" + std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

		TryCatch tryCatcher(env.isolate());

		Local<String> sourceStr = JS::string(env, code.c_str());

		ScriptOrigin origin(env.isolate(), JS::string(env, path.string().c_str()), 0, 0, true, -1, v8::Local<v8::Value>(), false, false, true);

		ScriptCompiler::Source source(sourceStr, origin);
		MaybeLocal<Module> maybeModule = ScriptCompiler::CompileModule(env.isolate(), &source);

		if (maybeModule.IsEmpty())
		{
			if (tryCatcher.HasCaught())
			{
				std::string exception = JS::parseString(env, tryCatcher.Exception());
				Logger::get().warn("Got exception while loading module ", filePath, "!\n", exception);
			}

			Logger::get().warn("Module ", path.string(), " is empty!");
			return v8::MaybeLocal<v8::Module>();
		}
		else if (tryCatcher.HasCaught())
		{
			std::string exception = JS::parseString(env, tryCatcher.Exception());
			Logger::get().warn("Got exception while loading module ", filePath, "!\n", exception);
		}
		else
		{
			Local<Module> module = maybeModule.ToLocalChecked();

			modulesPaths_.emplace(module->ScriptId(), path.string());
			modules_.emplace(path.string(), new Persistent<Module>(env.isolate(), module));

			Maybe<bool> result = module->InstantiateModule(env.context(), importModule);

			if (result.IsNothing())
			{
				Logger::get().warn("Could not instantiate module ", path.string(), "!");
				return module;
			}
			else
			{
				module->Evaluate(env.context());
				return module;
			}
		}
	}

	void ModuleLoader::loadEntryModule(const std::filesystem::path& filePath) const
	{
		using namespace v8;

		TryCatch tryCatcher(env.isolate());

		std::filesystem::path path;

		if (filePath.is_relative())
			path = (Engine::getInstance().gamePath / filePath).lexically_normal();

		std::string p = path.string();
		std::replace(p.begin(), p.end(), '\\', '/');

		std::string entry = path.string() + ".native.entry";

		std::string code;

		code = std::format("import entry from \"{}\"; {}", p, "entry(Worker.getParentWorker(), process.args);");

		Local<String> sourceStr = JS::string(env, code);
		ScriptOrigin origin(env.isolate(), JS::string(env, entry), 0, 0, true, -1, v8::Local<v8::Value>(), false, false, true);
		ScriptCompiler::Source source(sourceStr, origin);

		MaybeLocal<Module> maybeModule = ScriptCompiler::CompileModule(env.isolate(), &source);

		if (maybeModule.IsEmpty())
		{
			Logger::get().warn("Could not compile module ", p, "!");
			return;
		}
		else if (tryCatcher.HasCaught())
		{
			std::string exception = JS::parseString(env, tryCatcher.Exception());
			Logger::get().warn("Got exception while loading module ", p, "!\n", exception);
		}
		else
		{
			Local<Module> module = maybeModule.ToLocalChecked();

			modulesPaths_.emplace(module->ScriptId(), entry);
			modules_.emplace(entry, new Persistent<Module>(env.isolate(), module));

			Maybe<bool> result = module->InstantiateModule(env.context(), importModule);

			if (result.IsNothing())
			{
				Logger::get().warn("Could not instantiate module ", p, "!");
			}
			else
			{
				module->Evaluate(env.context());
			}
		}
	}
}