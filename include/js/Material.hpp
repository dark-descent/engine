#pragma once

#include "js/Class.hpp"

namespace DarkDescent::JS
{
	class Env;

	class MaterialClass: public Class
	{
		JS_CLASS_BODY(MaterialClass);

	private:
		JS_CLASS_METHOD(ctor);
		JS_CLASS_METHOD(clone);
		JS_CLASS_METHOD(set);
		JS_CLASS_METHOD(get);
	};
}