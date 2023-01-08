#pragma once

#include "SubSystem.hpp"
#include "pch.hpp"
#include "js/Env.hpp"
#include "js/Object.hpp"
#include "PersistentVector.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	class Scene;
	
	namespace JS
	{
		class Object;
	}

	class ScriptManager: public SubSystem
	{
	public:
		enum class Events : Hash
		{
			ENV_CREATED = Hasher::hash("ENV_CREATED"), // 1801904197
			ENV_DESTROYED = Hasher::hash("ENV_DESTROYED") // 676292800
		};

	private:
		static std::unique_ptr<v8::Platform> platform_;

		static void initializeV8();
		static void terminateV8();

		static inline bool isV8Initialized() { return platform_ != nullptr; }

		template<typename Callback>
		static void execStandAlone(Callback callback)
		{
			using namespace v8;

			if (!isV8Initialized())
				initializeV8();

			JS::Env env = JS::Env::create(0, nullptr, true);
			env.run(callback);
		}

	protected:
		SUB_SYSTEM_CTORS(ScriptManager) { };

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;
		
	public:
		const JS::Env& createEnv();
		void destroyEnv(const JS::Env& env);

		void setCurrentScene(Scene* scene);
		Scene* getCurrentScene();

		inline const JS::Env& mainEnv() const { return *envs_.at(0); }

		bool initializeGame();

	private:
		PersistentVector<JS::Env*, 8> envs_;
		Scene* currentScene_;
	};
}