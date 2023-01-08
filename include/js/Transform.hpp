#pragma once

#include "js/Class.hpp"

namespace DarkDescent::JS
{
	class Env;

	class TransformClass: public Class
	{
		JS_CLASS_BODY(TransformClass);

	private:
		JS_CLASS_METHOD(ctor);
	};
}