#include "SubSystem.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	SubSystem::SubSystem(const Engine& engine) : name_("class DarkDescent::SubSystem"), engine_(engine) { }
	SubSystem::~SubSystem() { }
	void SubSystem::initialize(const char* name)
	{
		name_ = name;
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
	void SubSystem::run() { }
}