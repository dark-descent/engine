#pragma once

#include "pch.hpp"
#include "ScriptManager.hpp"

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

		ScriptManager scriptManager_;
	};
}