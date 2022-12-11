#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"

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

	private:
		std::unique_ptr<v8::Platform> platform_;
		v8::Isolate::CreateParams create_params_;
		v8::Isolate* mainIsolate_;
	};
}