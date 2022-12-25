#include "js/Env.hpp"
#include "Logger.hpp"
#include "js/Console.hpp"
#include "js/Object.hpp"
#include "Engine.hpp"
#include "js/Helpers.hpp"

namespace DarkDescent::JS
{
	Env Env::create()
	{
		using namespace v8;
		v8::Isolate::CreateParams params;
		params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		return Env(std::move(params));
	}

	Env* Env::createNew()
	{
		using namespace v8;
		v8::Isolate::CreateParams params;
		params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		return new Env(std::move(params));
	}

	Env::Env(v8::Isolate::CreateParams&& createParams):
		createParams_(createParams),
		isolate_(v8::Isolate::New(createParams_))
	{
		assert(isolate_->GetNumberOfDataSlots() != 0);
		isolate_->SetData(0, this);
		isolate_->SetAbortOnUncaughtExceptionCallback([](v8::Isolate* isolate) -> bool {
			Logger::get().error("Uncaught js exception!");
			return true;
		});

		v8::Isolate::Scope isolateScope(isolate_);
		v8::HandleScope handleScope(isolate_);
		context_.Reset(isolate_, v8::Context::New(isolate_));
		auto ctx = context();
		v8::Context::Scope contextScope(ctx);

		JS::Object global(*this, ctx->Global());
		JS::Console::expose(*this, global);
	}

	Env::~Env()
	{
		Logger::get().info("Terminating JS::Env...");
		context_.Reset();
		isolate_->Dispose();
		delete createParams_.array_buffer_allocator;
	}

	v8::MaybeLocal<v8::Value> Env::readJson(const std::string& source) const
	{
		v8::Local<v8::String> jsonString = v8::String::NewFromUtf8(isolate_, source.data()).ToLocalChecked();
		return v8::JSON::Parse(context(), jsonString);
	}

	v8::MaybeLocal<v8::Value> Env::readJsonFile(const std::filesystem::path& path) const
	{
		std::ifstream is(path, std::ifstream::beg | std::ifstream::binary);
		std::ostringstream ss;
		ss << is.rdbuf();
		const std::string& s = ss.str();
		return readJson(s);
	}

	/*static*/ v8::MaybeLocal<v8::Module> Env::importModule(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer)
	{
		const int id = referrer->ScriptId();

		Env& env = *static_cast<Env*>(context->GetIsolate()->GetData(0));

		std::string import = JS::parseString(env, specifier);

		std::replace(import.begin(), import.end(), '\\', '/');

		std::filesystem::path importPath = import;
		std::filesystem::path fromPath;

		if (!importPath.is_relative())
		{
			fromPath = std::filesystem::path("");
		}
		else if (env.modulesPaths_.contains(id))
		{
			fromPath = (std::filesystem::path(env.modulesPaths_.at(id)) / "..").lexically_normal();
		}
		else
		{
			fromPath = Engine::getInstance().gamePath;
		}

		if (importPath.is_relative())
			importPath = fromPath / importPath;

		if (!std::filesystem::exists(importPath))
		{
			bool isValid = false;
			const std::vector<std::string> resolve = { ".js", ".jsx", ".json" };
			for (const std::string& t : resolve)
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
				Logger::get().warn("Could not resolve import path for ", importPath.string(), "!");
				return v8::MaybeLocal<v8::Module>();
			}
		}

		const std::string ext = importPath.extension().string();

		v8::MaybeLocal<v8::Module> module;

		if (ext.compare(".json") == 0)
		{
			module = env.loadJsonModule(importPath.string().c_str());
		}
		else
		{
			module = env.loadModule(importPath.string().c_str());
		}

		return v8::MaybeLocal<v8::Module>(module);
	}

	v8::MaybeLocal<v8::Module> Env::loadJsonModule(const char* filePath) const
	{
		using namespace v8;

		TryCatch tryCatcher(isolate());

		const std::string path(filePath);

		if (modules_.contains(path))
			return v8::MaybeLocal<v8::Module>();

		std::ifstream is(filePath);
		std::string jsonString((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

		std::vector<v8::Local<v8::String>> exports({ JS::string(*this, "default") });

		Local<Module> module = Module::CreateSyntheticModule(isolate(), JS::string(*this, filePath), exports, [](Local<Context> context, Local<Module> module)
		{
			Env& env = *static_cast<Env*>(context->GetIsolate()->GetData(0));
			module->SetSyntheticModuleExport(context->GetIsolate(), v8::String::NewFromUtf8(context->GetIsolate(), "default").ToLocalChecked(), env.getJsonData(module->GetIdentityHash()).ToLocalChecked());
			return MaybeLocal<Value>(v8::True(env.isolate()));
		});

		Local<Value> json = v8::JSON::Parse(context(), JS::string(*this, jsonString)).ToLocalChecked();

		jsonModules_.emplace(module->GetIdentityHash(), new Persistent<Value>(isolate(), json));
		modules_.emplace(path, new Persistent<Module>(isolate(), module));

		Maybe<bool> result = module->InstantiateModule(context(), Env::importModule);

		if (result.IsNothing())
		{
			Logger::get().error("Can't instantiate module.");
			return v8::MaybeLocal<v8::Module>();
		}
		else
		{
			MaybeLocal<Value> result = module->Evaluate(context());
			return v8::MaybeLocal<v8::Module>(module);
		}
	}

	v8::MaybeLocal<v8::Value> Env::getJsonData(const int moduleHash) const
	{
		if (jsonModules_.contains(moduleHash))
			return v8::MaybeLocal<v8::Value>(jsonModules_.at(moduleHash)->Get(isolate()));
		return v8::MaybeLocal<v8::Value>();
	}

	std::filesystem::path fixPath(const std::filesystem::path& path)
	{
		if (path.is_relative())
		{
			const auto& p = Engine::getInstance().gamePath;
			return (p / path).lexically_normal();
		}
		return path.lexically_normal();
	}

	v8::MaybeLocal<v8::Module> Env::loadModule(const std::filesystem::path& path)
	{
		using namespace v8;

		const Logger& logger = Logger::get();

		std::filesystem::path scriptPath = fixPath(path);

		std::string p = (scriptPath / "..").lexically_normal().string();
		std::replace(p.begin(), p.end(), '\\', '/');
		std::string fileName = scriptPath.filename().string();

		std::ifstream is(scriptPath);
		std::string code = std::string("const __dirname = \"") + p + "\"; const __filename = \"" + fileName + "\";" + std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

		TryCatch tryCatcher(isolate());

		Local<String> sourceStr = JS::string(*this, code.c_str());

		ScriptOrigin origin(isolate(), JS::string(*this, scriptPath.string().c_str()), 0, 0, true, -1, v8::Local<v8::Value>(), false, false, true);

		ScriptCompiler::Source source(sourceStr, origin);
		MaybeLocal<Module> maybeModule = ScriptCompiler::CompileModule(isolate(), &source);

		if (maybeModule.IsEmpty())
		{
			if (tryCatcher.HasCaught())
			{
				std::string exception = JS::parseString(*this, tryCatcher.Exception());
				std::string msg = std::format("Got exception while loading module {}! Exception: {}", scriptPath.string().c_str(), exception.c_str());
				throw TraceException(msg.c_str());
			}

			std::string msg = std::format("Module {} is empty!", path.string().c_str());
			throw TraceException(msg.c_str());
		}
		else if (tryCatcher.HasCaught())
		{
			std::string exception = JS::parseString(*this, tryCatcher.Exception());
			std::string msg = std::format("Got exception while loading module {}! Exception {}", scriptPath.string().c_str(), exception.c_str());
			throw TraceException(msg.c_str());
		}
		else
		{
			Local<Module> module = maybeModule.ToLocalChecked();

			modulesPaths_.emplace(module->ScriptId(), path.string());
			modules_.emplace(path.string(), new Persistent<Module>(isolate(), module));

			if (!module->InstantiateModule(context(), importModule).ToChecked())
			{
				logger.error("Could not instantiate module ", path.string(), "!");
				return module;
			}
			else
			{
				module->Evaluate(context());
				return module;
			}
		}
	}

	void Env::loadEntryModule(const std::filesystem::path& path) const
	{
		using namespace v8;

		const Logger& logger = Logger::get();

		std::filesystem::path scriptPath = fixPath(path);

		std::string p = (scriptPath / "..").lexically_normal().string();
		std::replace(p.begin(), p.end(), '\\', '/');
		std::string fileName = scriptPath.filename().string();

		std::ifstream is(scriptPath);
		
		TryCatch tryCatcher(isolate());

		std::string sp = p + fileName;

		std::string code = std::format("import main from \"{}\"; {}", sp.c_str(), "main();");
		
		std::string entry = sp + ".native.entry";

		Local<String> sourceStr = JS::string(*this, code);
		ScriptOrigin origin(isolate(), JS::string(*this, entry), 0, 0, true, -1, v8::Local<v8::Value>(), false, false, true);
		ScriptCompiler::Source source(sourceStr, origin);

		MaybeLocal<Module> maybeModule = ScriptCompiler::CompileModule(isolate(), &source);

		if (maybeModule.IsEmpty())
		{
			std::string msg = std::format("Could not compile module {}!", scriptPath.string());
			throw TraceException(msg.c_str());
		}
		else if (tryCatcher.HasCaught())
		{
			std::string exception = JS::parseString(*this, tryCatcher.Exception());
			std::string msg = std::format("Got exception while loading module {}! Exception: {}", scriptPath.string().c_str(), exception.c_str());
			throw TraceException(msg.c_str());
		}
		else
		{
			Local<Module> module = maybeModule.ToLocalChecked();

			modulesPaths_.emplace(module->ScriptId(), entry);
			modules_.emplace(entry, new Persistent<Module>(isolate(), module));

			Maybe<bool> result = module->InstantiateModule(context(), importModule);

			if (result.IsNothing())
			{
				std::string msg = std::format("Could not load entry module {}!", scriptPath.string());
				throw TraceException(msg.c_str());
			}
			else
			{
				module->Evaluate(context());
			}
		}
	}

}