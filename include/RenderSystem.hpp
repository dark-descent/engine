#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

namespace DarkDescent
{
	class RenderSystem: public SubSystem
	{
		SUB_SYSTEM_CTORS(RenderSystem);

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;
	};
}