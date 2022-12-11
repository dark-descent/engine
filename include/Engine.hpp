#pragma once

#include "pch.hpp"
#include "ScriptManager.hpp"
#include "Hash.hpp"
#include "Logger.hpp"
#include "SubSystem.hpp"

namespace DarkDescent
{
	class Engine
	{
	private:
		static std::optional<Engine*> instance_;

	public:
		static Engine& initialize();
		static Engine& getInstance();
		static bool terminate();

	private:
		Engine();
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
		const Logger& logger;
	private:

		ScriptManager scriptManager_;
		std::vector<SubSystem*> initializedSystems_;
	};
}