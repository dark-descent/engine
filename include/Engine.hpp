#pragma once

#include "pch.hpp"
#include "Hash.hpp"
#include "Logger.hpp"
#include "SubSystem.hpp"
#include "Config.hpp"
#include "TraceException.hpp"
#include "EventManager.hpp"

namespace DarkDescent
{
	class Engine
	{
	private:
		static std::optional<Engine*> instance_;

	public:
		static Engine& initialize(std::vector<const char*>&& args);
		static Engine& getInstance();
		static bool terminate();

	private:
		Engine(std::filesystem::path&&, std::vector<const char*>&&);
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		~Engine();

		template<IsSubSystem T>
		T& initializeSubSystem()
		{
			const char* name = typeid(T).name();
			Hash hash = Hasher::hash(name);

			assert(!subSystems_.contains(hash));

			T* subSystem = new T(name, *this);
			subSystems_.emplace(hash, subSystem);
			initializationOrder_.emplace_back(subSystem);
			subSystem->initialize();
			return *subSystem;
		}

	public:
		void run();

		template<IsSubSystem T>
		T* getSubSystem() const
		{
			const char* name = typeid(T).name();
			Hash hash = Hasher::hash(name);

			if (!subSystems_.contains(hash))
			{
				std::string error = std::format("Could not get subsystem for type {}!", name);
				throw TraceException(error.c_str());
			}

			return static_cast<T*>(subSystems_.at(hash));
		}

		const Config& config() const { return config_; }

	public:
		const std::filesystem::path gamePath;
		const std::thread::id mainThreadID;
		const Logger& logger;
		const EventManager eventManager;
		const std::vector<const char*> gameArgs;

	private:
		std::unordered_map<Hash, SubSystem*> subSystems_;
		std::vector<SubSystem*> initializationOrder_;
		Config config_;
	};
}