#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"
#include "js/Env.hpp"

namespace DarkDescent
{
	class ScriptManager: public SubSystem
	{
	private:
		static std::unique_ptr<v8::Platform> platform_;

		static void initializeV8();
		static void terminateV8();

		static inline bool isV8Initialized() { return platform_ != nullptr; }
		
		template<typename Callback>
		static void execStandAlone(Callback callback)
		{
			using namespace v8;

			if(!isV8Initialized())
				initializeV8();

			JS::Env* env = JS::Env::createNew();
			env->run(callback);
			delete env;
		}

	protected:
		SUB_SYSTEM_CTORS(ScriptManager);

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;

		inline JS::Env& mainEnv() const { return *(envs_.at(0)); }

	private:
		
		std::vector<JS::Env*> envs_;

	};
}