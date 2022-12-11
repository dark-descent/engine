#include "Engine.hpp"
#include "TraceException.hpp"
#include "Logger.hpp"

namespace DarkDescent
{
	std::optional<Engine*> Engine::instance_;

	Engine& Engine::initialize()
	{
		if (instance_.has_value())
			throw TraceException("Engine is already initialized!");
		instance_.emplace(new Engine());
		return *(instance_.value());
	}

	Engine& Engine::getInstance()
	{
		if (!instance_.has_value())
			throw TraceException("Engine is not initialized yet!");

		return *(instance_.value());
	}

	bool Engine::terminate()
	{
		if (!instance_.has_value())
			throw TraceException("Engine is not initialized!");

		Engine* engine = instance_.value();
		delete engine;
		instance_.reset();
		return true;
	}

	Engine::Engine():
		logger(Logger::get()),
		mainThreadID(std::this_thread::get_id()),
		scriptManager_(*this),
		initializedSystems_()
	{
		const Logger& logger = Logger::get();
		logger.info("Initializing engine...");

		initializeSubSystem(scriptManager_);

		logger.info("Engine initialized!");
	}

	Engine::~Engine()
	{
		const Logger& logger = Logger::get();
		logger.info("Terminating engine...");

		for(int i = static_cast<int>(initializedSystems_.size()) - 1; i >= 0; i--)
		{
			initializedSystems_[i]->terminate();
		}

		initializedSystems_.clear();

		logger.info("Engine terminated!");
	}
}