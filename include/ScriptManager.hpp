#pragma once

#include "SubSystem.hpp"

namespace DarkDescent
{
	class ScriptManager: public SubSystem
	{
	protected:
		SUB_SYSTEM_CTORS(ScriptManager);

	protected:
		virtual void onInitialize() override { }
		virtual void onTerminate() override { }
		virtual void run() override { }
	};
}