#pragma once

#include "pch.hpp"
#include "ScriptManager.hpp"
#include "Hash.hpp"
#include "Logger.hpp"
#include "SubSystem.hpp"
#include "Config.hpp"
#include "WindowManager.hpp"

namespace DarkDescent
{
	class Engine
	{
	private:
		static std::optional<Engine*> instance_;

	public:
		static Engine& initialize(const char* gamePath = nullptr);
		static Engine& getInstance();
		static bool terminate();

	private:
		Engine(Config&&);
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		~Engine();

		template<typename T>
			requires IsSubSystem<T>
		void initializeSubSystem(T& subSystem)
		{
			subSystem.initialize(typeid(T).name());
			initializedSystems_.emplace_back(std::addressof(subSystem));
		}

	public:
		void run();

	public:
		const std::thread::id mainThreadID;
		const Logger& logger;
		const Config config;

	private:
		ScriptManager scriptManager_;
		WindowManager windowManager_;
		std::vector<SubSystem*> initializedSystems_;
	};
}