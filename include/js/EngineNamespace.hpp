#pragma once

namespace DarkDescent::JS
{
	class Env;
	class Object;

	namespace Engine
	{
		v8::Local<v8::Object> createNamespace(const JS::Env& env);
	}
}