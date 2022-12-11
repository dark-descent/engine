#pragma once

#include "SubSystem.hpp"

namespace DarkDescent
{
	class ScriptManager: public SubSystem
	{
	protected:
		SUB_SYSTEM_CTORS(ScriptManager);

	protected:
		virtual void initialize() { }
		virtual void terminate() { }
		virtual void run() { }
	};
}