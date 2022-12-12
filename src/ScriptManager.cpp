#include "ScriptManager.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	std::unique_ptr<v8::Platform> ScriptManager::platform_ = nullptr;

	void ScriptManager::initializeV8()
	{
		assert(!isV8Initialized());
		platform_ = v8::platform::NewDefaultPlatform();
		v8::V8::InitializePlatform(platform_.get());
		v8::V8::Initialize();
	}

	void ScriptManager::terminateV8()
	{
		assert(isV8Initialized());
		v8::V8::Dispose();
		v8::V8::DisposePlatform();
		platform_.release();
	}

	void ScriptManager::onInitialize()
	{
		if(!isV8Initialized())
			initializeV8();
		
		envs_.emplace_back(JS::Env::createNew());
	}

	void ScriptManager::onTerminate()
	{
		for (auto env : envs_)
			delete env;
		envs_.clear();
	}
}