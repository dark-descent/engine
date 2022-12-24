#include "SubSystem.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	SubSystem::SubSystem(const char* name, const Engine& engine) : name_(name), engine_(engine) { }
	SubSystem::~SubSystem() { }

	void SubSystem::initialize()
	{
		engine_.logger.info("Initializing ", name_, "...");
		onInitialize();
		engine_.logger.info(name_, " initialized!");
	}

	void SubSystem::terminate()
	{
		engine_.logger.info("Terminating ", name_, "...");
		onTerminate();
		engine_.logger.info(name_, " terminated!");
	}

	void SubSystem::onInitialize() { }
	void SubSystem::onTerminate() { }

	void SubSystem::emitEvent(const char* name, const void* data)
	{
		emitEvent(Hasher::hash(name), data);
	}

	void SubSystem::addEventHandler(const char* name, EventHandler eventHandler, const void* data)
	{
		addEventHandler(Hasher::hash(name), eventHandler, data);
	}

	void SubSystem::removeEventHandler(const char* name, EventHandler eventHandler)
	{
		removeEventHandler(Hasher::hash(name), eventHandler);
	}

	void SubSystem::emitEvent(Hash name, const void* data)
	{
		Event e { this, data };
		engine_.eventManager.emit(name, e);
	}

	void SubSystem::addEventHandler(Hash name, EventHandler eventHandler, const void* data)
	{
		engine_.eventManager.on(name, this, eventHandler, data);
	}

	void SubSystem::removeEventHandler(Hash name,  EventHandler eventHandler)
	{
		engine_.eventManager.remove(name, eventHandler);
	}
}