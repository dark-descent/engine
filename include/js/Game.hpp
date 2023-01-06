#pragma once

#include "js/Class.hpp"
#include "Window.hpp"

namespace DarkDescent
{
	class Window;
	namespace JS
	{
		class Env;

		class Game: public ObjectWrapper
		{
		public:
			enum class InternalIndex: int
			{
				Engine = 0,
				Window = 1,
			};

			Game(const Env& env);
			virtual ~Game();

			virtual void initializeProps();

			JS_METHOD_DECL(onInitialize);
			JS_METHOD_DECL(onLoad);
			JS_METHOD_DECL(onTerminate);

			inline bool hasWindow() const noexcept
			{
				return !getInternal(InternalIndex::Window).IsEmpty();
			}

			void setWindow(v8::Local<v8::Value> window) const noexcept
			{
				JS::Object gameObj(env_, value());
				gameObj.set("window", window, v8::PropertyAttribute::ReadOnly);
			}
		};

		class GameClass: public Class
		{
			JS_CLASS_BODY(GameClass);

		private:
			JS_CLASS_METHOD(ctor);
			// JS_CLASS_METHOD(onInitialize);
			// JS_CLASS_METHOD(onLoad);
			// JS_CLASS_METHOD(onTerminate);
		};
	}
}