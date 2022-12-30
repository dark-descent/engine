#include "SubSystem.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	SubSystem::Logger::Logger(const char* name):
		prefix_(std::format("[{}] ", name)),
		logger_(DarkDescent::Logger::get())
	{ }

	SubSystem::Logger::~Logger() { }


	SubSystem::SubSystem(const char* name, const Engine& engine):
		name(name),
		logger(name),
		engine_(engine)
	{ }

	SubSystem::~SubSystem() { }

	void SubSystem::initialize()
	{
		logger.info("initializing...");
		onInitialize();
	}

	void SubSystem::allInitialized()
	{
		onReady();
	}

	void SubSystem::terminate()
	{
		logger.info("terminating...");
		onTerminate();
	}

	void SubSystem::onInitialize() { }
	void SubSystem::onReady() { }
	void SubSystem::onTerminate() { }

	void SubSystem::emitEvent(const char* name, void* data)
	{
		emitEvent(Hasher::hash(name), data);
	}

	void SubSystem::addEventHandler(const char* name, EventHandler eventHandler, void* data)
	{
		addEventHandler(Hasher::hash(name), eventHandler, data);
	}

	void SubSystem::removeEventHandler(const char* name, EventHandler eventHandler)
	{
		removeEventHandler(Hasher::hash(name), eventHandler);
	}

	void SubSystem::emitEvent(Hash name, void* data)
	{
		Event e{ this, data };
		engine_.eventManager.emit(name, e);
	}

	void SubSystem::addEventHandler(Hash name, EventHandler eventHandler, void* data)
	{
		engine_.eventManager.on(name, this, eventHandler, data);
	}

	void SubSystem::removeEventHandler(Hash name, EventHandler eventHandler)
	{
		engine_.eventManager.remove(name, eventHandler);
	}
}