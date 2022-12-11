#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"
#include "js/Env.hpp"

namespace DarkDescent
{
	class ScriptManager: public SubSystem
	{
	protected:
		SUB_SYSTEM_CTORS(ScriptManager);

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;
		virtual void run() override;

		inline JS::Env& mainEnv() const { return *(envs_.at(0)); }

	private:
		std::unique_ptr<v8::Platform> platform_;
		std::vector<JS::Env*> envs_;
		
	};
}