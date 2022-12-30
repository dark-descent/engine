#pragma once

#include "pch.hpp"

namespace DarkDescent::JS
{
	class Env;
	
	namespace EngineModule
	{
		bool isImportString(const std::string& str);
		v8::Local<v8::Module> create(const Env& env);
	}
}