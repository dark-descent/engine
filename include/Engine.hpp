#pragma once

#include "pch.hpp"
#include "Hash.hpp"
#include "Logger.hpp"
#include "SubSystem.hpp"
#include "Config.hpp"
#include "TraceException.hpp"
#include "EventManager.hpp"
#include "js/Game.hpp"

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
		JS::Game& initializeGame(const JS::Env& env, v8::Local<v8::Object> game, const v8::FunctionCallbackInfo<v8::Value>& args);

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
		const std::optional<JS::Game>& game() const { return game_; }

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
		std::optional<JS::Game> game_;
	};
}