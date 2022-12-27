#pragma once

#include "pch.hpp"
#include "js/ModuleLoader.hpp"

namespace DarkDescent
{
	class ScriptManager;
	
	namespace JS
	{
		class Env
		{
		public:
			struct Scope
			{
				Scope(Env& env):
					isolateScope_(env.isolate_),
					handleScope_(env.isolate_),
					contextScope_(env.context())
				{ }

				const v8::Isolate::Scope isolateScope_;
				const v8::HandleScope handleScope_;
				const v8::Context::Scope contextScope_;
			};

			static Env create(const bool standAlone = false);
			static Env* createNew(const bool standAlone = false);

			static inline const Env& fromIsolate(v8::Isolate* isolate) { return *static_cast<Env*>(isolate->GetData(0)); }
			static inline const Env& fromContext(v8::Local<v8::Context> ctx) { return fromIsolate(ctx->GetIsolate()); }
			static inline const Env& fromArgs(const v8::FunctionCallbackInfo<v8::Value>& args) { return fromIsolate(args.GetIsolate()); }

		private:
			Env(v8::Isolate::CreateParams&& createParams, const bool isStandAloneEnv = false);
			Env(const Env&) = delete;
			Env(Env&&) = delete;

		public:
			~Env();

		private:
			void loadEntryModule(const std::filesystem::path&);
		public:

			inline v8::Isolate* isolate() const { return isolate_; }
			inline v8::Local<v8::Context> context() const { return context_.Get(isolate_); }

			v8::MaybeLocal<v8::Value> readJson(const std::string& source) const;
			v8::MaybeLocal<v8::Value> readJsonFile(const std::filesystem::path& jsonPath) const;

			template<typename Callback>
			void run(Callback callback)
			{
				Scope scope(*this);
				callback(*this);
			}

			bool isLoaded() const { return isLoaded_; }
			inline const ModuleLoader& moduleLoader() const { return moduleLoader_; }

		private:
			v8::Isolate::CreateParams createParams_;
			v8::Isolate* isolate_;
			v8::Global<v8::Context> context_;

			ModuleLoader moduleLoader_;
			mutable bool isLoaded_;

			friend class ScriptManager;
		};
	}
}