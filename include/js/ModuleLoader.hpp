#pragma once

#include "pch.hpp"

namespace DarkDescent::JS
{
	class Env;

	class ModuleLoader
	{
	private:
		struct ScriptPathInfo
		{
		public:
			ScriptPathInfo(const std::filesystem::path& path): path_(path) { }

			inline const std::filesystem::path& path() const { return path_; };
			inline std::filesystem::path filename() const { return path_.filename(); };
			inline std::filesystem::path dir() const
			{
				std::filesystem::path clone = path_;
				clone.remove_filename();
				return clone;
			};

		private:
			std::filesystem::path path_;
		};

	private:
		static v8::MaybeLocal<v8::Module> importModule(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer);


	public:
		ModuleLoader(const Env& env);
		ModuleLoader(const ModuleLoader&) = delete;
		ModuleLoader(ModuleLoader&&) = delete;
		~ModuleLoader();

		v8::MaybeLocal<v8::Value> getJsonData(const int moduleHash) const;
		v8::MaybeLocal<v8::Module> loadModule(const char* filePath) const;
		v8::MaybeLocal<v8::Module> loadJsonModule(const char* filePath) const;
		void loadEntryModule(const std::filesystem::path& filePath) const;

	// 	v8::MaybeLocal<v8::Module> loadModule(const std::filesystem::path& path) const;
	// 	void loadEntryModule(const std::filesystem::path& path) const;


	// private:
	// 	bool compileModule(const std::filesystem::path& path, const std::string& code, v8::Local<v8::Module>& module) const;
	// 	ScriptPathInfo resolveScriptPath(const std::filesystem::path& path) const;

		const Env& env;

		mutable std::array<std::filesystem::path, 3> checkPaths_;
		bool isLoaded_;
		
		mutable std::unordered_map<int, std::string> modulesPaths_;
		mutable std::unordered_map<std::string, v8::Persistent<v8::Module>*> modules_;
		mutable std::unordered_map<int, v8::Persistent<v8::Value>*> jsonModules_;

	};
}