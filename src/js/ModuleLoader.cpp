#include "js/ModuleLoader.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "js/Helpers.hpp"
#include "js/Console.hpp"

namespace DarkDescent::JS
{
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

	v8::MaybeLocal<v8::Module> ModuleLoader::loadJsonModule(const std::filesystem::path& filePath) const
	{
		v8::TryCatch tryCatcher(env.isolate());

		const std::string path = filePath.string();

		if (modules_.contains(path))
			return v8::MaybeLocal<v8::Module>();

		std::ifstream is(filePath);
		std::string jsonString((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

		std::vector<v8::Local<v8::String>> exports({ JS::string(env, "default") });

		v8::Local<v8::Module> module =v8::Module::CreateSyntheticModule(env.isolate(), JS::string(env, path), exports, [](v8::Local<v8::Context> context, v8::Local<v8::Module> module)
		{
			Env& env = *static_cast<Env*>(context->GetIsolate()->GetData(0));
			module->SetSyntheticModuleExport(context->GetIsolate(), v8::String::NewFromUtf8(context->GetIsolate(), "default").ToLocalChecked(), env.moduleLoader().getJsonData(module->GetIdentityHash()).ToLocalChecked());
			return v8::MaybeLocal<v8::Value>(v8::True(env.isolate()));
		});

		v8::Local<v8::Value> json = v8::JSON::Parse(env.context(), JS::string(env, jsonString)).ToLocalChecked();

		jsonModules_.emplace(module->GetIdentityHash(), new v8::Persistent<v8::Value>(env.isolate(), json));
		modules_.emplace(path, new v8::Persistent<v8::Module>(env.isolate(), module));

		v8::Maybe<bool> result = module->InstantiateModule(env.context(), ModuleLoader::importModule);

		if (result.IsNothing())
		{
			Logger::get().error("Can't instantiate module.");
			return v8::MaybeLocal<v8::Module>();
		}
		else
		{
			v8::MaybeLocal<v8::Value> result = module->Evaluate(env.context());
			return v8::MaybeLocal<v8::Module>(module);
		}
	}

	v8::MaybeLocal<v8::Value> ModuleLoader::getJsonData(const int moduleHash) const
	{
		if (jsonModules_.contains(moduleHash))
			return v8::MaybeLocal<v8::Value>(jsonModules_.at(moduleHash)->Get(env.isolate()));
		return v8::MaybeLocal<v8::Value>();
	}

	v8::MaybeLocal<v8::Module> ModuleLoader::loadModule(const std::filesystem::path& filePath) const
	{
		using namespace v8;

		if (filePath.is_relative())
			throw TraceException("Cannot load module with a relative path!");

		std::filesystem::path path = filePath;
		std::string p = (path / "..").lexically_normal().string();
		std::replace(p.begin(), p.end(), '\\', '/');
		std::string fileName = path.filename().string();

		std::ifstream is(path);
		std::string code = std::format("const __dirname = \"{}\"; const __filename = \"{}\";", p.c_str(), fileName.c_str()) + std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		return instantiateModule(path.string(), code);
	}

	void ModuleLoader::initialize(const std::filesystem::path& filePath) const
	{
		std::filesystem::path path;

		if (filePath.is_relative())
			path = (Engine::getInstance().gamePath / filePath).lexically_normal();
		else
			path = filePath;

		std::string p = path.string();
		std::replace(p.begin(), p.end(), '\\', '/');

		std::string entry = p + ".native.entry";

		std::string code = std::format("import entry from \"{}\"; {}", p, "entry();");

		if(instantiateModule(p, code, entry).IsEmpty())
			throw TraceException("Could not instantiate entry module!");
	}

	v8::MaybeLocal<v8::Module> ModuleLoader::instantiateModule(const std::string& path, const std::string& code) const
	{
		return instantiateModule(path, code, path);
	}
	
	v8::MaybeLocal<v8::Module> ModuleLoader::instantiateModule(const std::string& path, const std::string& code, const std::string& entry) const
	{
		v8::TryCatch tryCatcher(env.isolate());

		v8::Local<v8::String> sourceStr = JS::string(env, code.c_str());

		v8::ScriptOrigin origin(env.isolate(), JS::string(env, entry.c_str()), 0, 0, true, -1, v8::Local<v8::Value>(), false, false, true);

		v8::ScriptCompiler::Source source(sourceStr, origin);
		v8::MaybeLocal<v8::Module> maybeModule = v8::ScriptCompiler::CompileModule(env.isolate(), &source);

		if (tryCatcher.HasCaught())
		{
			Console::logException(env, tryCatcher.Exception());
			std::string exception = std::format("Got exception while loading module {}! Exception: {}", path, JS::parseString(env, tryCatcher.Exception()));
			throw TraceException(exception.c_str());
		}
		else if (maybeModule.IsEmpty())
		{
			std::string exception = std::format("Module {} is empty!", path.c_str());
			throw TraceException(exception.c_str());
		}
		else
		{
			v8::Local<v8::Module> module = maybeModule.ToLocalChecked();

			modulesPaths_.emplace(module->ScriptId(), entry);
			modules_.emplace(entry, new v8::Persistent<v8::Module>(env.isolate(), module));

			v8::Maybe<bool> result = module->InstantiateModule(env.context(), importModule);
			if (tryCatcher.HasCaught())
			{
				Console::logException(env, tryCatcher.Exception());
				std::string exception = std::format("Got exception while instantiating module {}! Exception: {}", path, JS::parseString(env, tryCatcher.Exception()));
				throw TraceException(exception.c_str());
			}
			else if (result.IsNothing())
			{
				std::string exception = std::format("Could not instantiate module {}!", path.c_str());
				throw TraceException(exception.c_str());
			}
			else
			{
				module->Evaluate(env.context());
				if (tryCatcher.HasCaught())
				{
					Console::logException(env, tryCatcher.Exception());
					std::string exception = std::format("Got exception while evaluating module {}! Exception: {}", path, JS::parseString(env, tryCatcher.Exception()));
					throw TraceException(exception.c_str());
				}
				else
				{
					return module;
				}
			}
		}
	}
}