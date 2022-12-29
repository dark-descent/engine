#pragma once

#include "pch.hpp"
#include "js/ModuleLoader.hpp"
#include "js/Object.hpp"

namespace DarkDescent
{
	class ResourceManager;
	class ScriptManager;

	namespace JS
	{
		class Class;
		class Object;

		class Env
		{
		public:
			struct Scope
			{
				Scope(const Env& env):
					isolateScope_(env.isolate_),
					handleScope_(env.isolate_),
					contextScope_(env.context())
				{ }

				const v8::Isolate::Scope isolateScope_;
				const v8::HandleScope handleScope_;
				const v8::Context::Scope contextScope_;
			};

		private:
			static Env create(std::size_t index, ResourceManager& resourceManager);
			static Env* createNew(std::size_t index, ResourceManager& resourceManager);

		public:
			static inline const Env& fromIsolate(v8::Isolate* isolate) { return *static_cast<Env*>(isolate->GetData(0)); }
			static inline const Env& fromContext(v8::Local<v8::Context> ctx) { return fromIsolate(ctx->GetIsolate()); }
			static inline const Env& fromArgs(const v8::FunctionCallbackInfo<v8::Value>& args) { return fromIsolate(args.GetIsolate()); }

		private:
			Env(v8::Isolate::CreateParams&& createParams, std::size_t index, ResourceManager& resourceManager);
			Env(const Env&) = delete;
			Env(Env&&) = delete;

		public:
			~Env();

		private:
			void loadEntryModule(const std::filesystem::path&);
		public:

			inline v8::Isolate* isolate() const { return isolate_; }
			inline v8::Local<v8::Context> context() const { return context_.Get(isolate_); }
			inline JS::Object global() const { return JS::Object(*this, context()->Global()); }
			inline JS::Object engineNamespace() const { return JS::Object(*this, engineNamespace_.Get(isolate_)); }

			v8::MaybeLocal<v8::Value> readJson(const std::string& source) const;
			v8::MaybeLocal<v8::Value> readJsonFile(const std::filesystem::path& jsonPath) const;

			void throwException(const char* error) const;

			template<typename Callback>
			void run(Callback callback) const
			{
				Scope scope(*this);
				callback(*this);
			}

			template<typename T>
			v8::Local<v8::Function> registerClass() const
			{
				T* jsClass = new T(*this);
				jsClass->initialize();
				classes_.insert({ typeid(T).name(), jsClass });
				return jsClass->getClass();
			}

			template<typename T>
			T& getClass() const
			{
				return *static_cast<T*>(classes_.at(typeid(T).name()));
			}
			
			inline bool isLoaded() const { return isLoaded_; }
			inline const ModuleLoader& moduleLoader() const { return moduleLoader_; }

			const std::size_t index;

		private:
			v8::Isolate::CreateParams createParams_;
			v8::Isolate* isolate_;
			v8::Global<v8::Context> context_;
			v8::Global<v8::Object> engineNamespace_;

			ModuleLoader moduleLoader_;
			mutable bool isLoaded_;

			mutable std::unordered_map<const char*, Class*> classes_;

			friend class ScriptManager;
		};
	}
}