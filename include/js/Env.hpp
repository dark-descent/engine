#pragma once

#include "pch.hpp"

namespace DarkDescent
{
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

			static Env create();
			static Env* createNew();

			static inline const Env& fromIsolate(v8::Isolate* isolate) { return *static_cast<Env*>(isolate->GetData(0)); }
			static inline const Env& fromContext(v8::Local<v8::Context> ctx) { return fromIsolate(ctx->GetIsolate()); }
			static inline const Env& fromArgs(const v8::FunctionCallbackInfo<v8::Value>& args) { return fromIsolate(args.GetIsolate()); }
		private:
			static v8::MaybeLocal<v8::Module> importModule(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer);

		private:
			Env(v8::Isolate::CreateParams&& createParams);
			Env(const Env&) = delete;
			Env(Env&&) = delete;
		public:
			~Env();

			inline v8::Isolate* isolate() const { return isolate_; }
			inline v8::Local<v8::Context> context() const { return context_.Get(isolate_); }

			v8::MaybeLocal<v8::Value> readJson(const std::string& source) const;
			v8::MaybeLocal<v8::Value> readJsonFile(const std::filesystem::path& jsonPath) const;

			v8::MaybeLocal<v8::Value> getJsonData(const int moduleHash) const;
			v8::MaybeLocal<v8::Module> loadModule(const std::filesystem::path& path);
			v8::MaybeLocal<v8::Module> loadJsonModule(const char* filePath) const;
			void loadEntryModule(const std::filesystem::path& path) const;

			template<typename Callback>
			void run(Callback callback)
			{
				Scope scope(*this);
				callback(*this);
			}

			bool isLoaded() const { return isLoaded_; }

		private:
			v8::Isolate::CreateParams createParams_;
			v8::Isolate* isolate_;
			v8::Global<v8::Context> context_;

			mutable std::unordered_map<int, std::string> modulesPaths_;
			mutable std::unordered_map<std::string, v8::Persistent<v8::Module>*> modules_;
			mutable std::unordered_map<int, v8::Persistent<v8::Value>*> jsonModules_;
			mutable bool isLoaded_;
		};
	}
}