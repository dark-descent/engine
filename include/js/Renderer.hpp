#pragma once

namespace DarkDescent::JS
{
	class Env;
	class Object;

	namespace Renderer
	{
		v8::Local<v8::Object> createNamespace(const JS::Env& env);
	}
}