#include "ScriptManager.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	void ScriptManager::onInitialize()
	{
		using namespace v8;
		platform_ = platform::NewDefaultPlatform();
		V8::InitializePlatform(platform_.get());
		V8::Initialize();

		envs_.emplace_back(JS::Env::createNew());

		mainEnv().run([ & ](JS::Env& env)
		{
			char pBuf[256];
			size_t len = sizeof(pBuf);

			DWORD bytes = GetModuleFileNameA(NULL, pBuf, len);
			if (bytes)
			{
				std::filesystem::path jsonPath = (std::filesystem::path(pBuf) / ".." / ".." / ".." / ".." / "test.json").lexically_normal();
				if (!std::filesystem::exists(jsonPath))
					throw TraceException("test.json not found!");

				auto val = env.readJson(jsonPath).ToLocalChecked().As<v8::Object>();
				auto helloVal = val->Get(env.context(), v8::String::NewFromUtf8(env.isolate(), "hello").ToLocalChecked()).ToLocalChecked().As<v8::String>();
				v8::String::Utf8Value utfString(env.isolate(), helloVal);
				engine_.logger.debug("Hello: ", std::string(*utfString));
			}
		});
	}

	void ScriptManager::onTerminate()
	{
		for (auto env : envs_)
			delete env;
		envs_.clear();

		v8::V8::Dispose();
		v8::V8::DisposePlatform();
		platform_.release();
	}

	void ScriptManager::run()
	{

	}
}