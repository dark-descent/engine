#pragma once

#include "js/Class.hpp"

namespace DarkDescent::JS
{
	class Env;

	class GameObjectClass: public Class
	{
		JS_CLASS_BODY(GameObjectClass);

	private:
		JS_CLASS_METHOD(ctor);
		JS_CLASS_METHOD(addComponent);
		JS_CLASS_METHOD(getComponent);
		JS_CLASS_METHOD(removeComponent);
	};
}