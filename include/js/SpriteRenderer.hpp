#pragma once

#include "js/Class.hpp"

namespace DarkDescent::JS
{
	class Env;

	class SpriteRendererClass: public Class
	{
		JS_CLASS_BODY(SpriteRendererClass);

	private:
		JS_CLASS_METHOD(ctor);
	};
}