#pragma once

#include "js/Class.hpp"

namespace DarkDescent::JS
{
	class Env;

	class Game: public ObjectWrapper
	{
	public:
		Game(const Env& env);
		virtual ~Game();

		virtual void initializeProps();

		JS_METHOD_DECL(onInitialize);
		JS_METHOD_DECL(onLoad);
		JS_METHOD_DECL(onTerminate);
	};

	class GameClass: public Class
	{
		JS_CLASS_BODY(GameClass);

	private:
		JS_CLASS_METHOD(ctor);
		JS_CLASS_METHOD(onInitialize);
		JS_CLASS_METHOD(onLoad);
		JS_CLASS_METHOD(onTerminate);
	};
}