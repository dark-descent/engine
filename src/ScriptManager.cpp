#include "ScriptManager.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"
#include "ResourceManager.hpp"
#include "Script.hpp"

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
		if (!isV8Initialized())
			initializeV8();
	}

	void ScriptManager::onReady()
	{
		engine_.getSubSystem<ResourceManager>()->registerResourceType<Script, const char*>();
		createEnv();
	}

	void ScriptManager::onTerminate()
	{
		envs_.foreach([ & ](const JS::Env* env, std::size_t index)
		{
			destroyEnv(*env);
		});
		envs_.clear();
		terminateV8();
	}

	const JS::Env& ScriptManager::createEnv()
	{
		JS::Env* env = envs_.emplace_back(JS::Env::createNew(envs_.getNextIndex(), *engine_.getSubSystem<ResourceManager>()));
		JS::Env::Scope scope(*env);
		emitEvent(Events::ENV_CREATED, env);
		return *env;
	}

	void ScriptManager::destroyEnv(const JS::Env& env)
	{
		{
			JS::Env::Scope scope(env);
			emitEvent(Events::ENV_DESTROYED, std::addressof(*envs_.at(env.index)));
		}
		
		env.~Env();
		envs_.erase_at(env.index);
	}

	void ScriptManager::initializeGame()
	{
		mainEnv().run([ & ](const JS::Env& env)
		{
			env.moduleLoader_.initialize(engine_.config().entry);
		});
	}
}