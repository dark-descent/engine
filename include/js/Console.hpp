#pragma once

#include "pch.hpp"
#include "js/Object.hpp"

namespace DarkDescent
{
	namespace JS
	{
		class Env;

		namespace Console
		{
			std::string parse(const Env& env, v8::Local<v8::Value> val, size_t indentCount = 0, bool isObjectVal = false, bool skipIndent = false);
			void logValue(const Env& env, v8::Local<v8::Value> val);
			void logException(const Env& env, v8::Local<v8::Value> val);

			void expose(const Env& env, JS::Object& global);
		}
	};
};
