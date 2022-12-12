#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	namespace JS
	{
		class Env;

		namespace Format
		{
			std::string parse(const Env& env, v8::Local<v8::Value> val, size_t indentCount = 0, bool isObjectVal = false, bool skipIndent = false);
		}
	};
};