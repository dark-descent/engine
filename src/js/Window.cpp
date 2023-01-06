#include "pch.hpp"
#include "js/Window.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"
#include "WindowManager.hpp"

namespace DarkDescent::JS
{
	Window::Window(const Env& env) : ObjectWrapper(env) { }

	Window::~Window() { }

	void Window::initializeProps()
	{
	}


	JS_CLASS_METHOD_IMPL(WindowClass::ctor)
	{
		if(args.Length() >= 0 && args[0]->IsExternal()) [[likely]]
		{
			JS::setInternal(args.This(), 0, args[0]);
		}
		else [[unlikely]]
		{
			env.throwException("Window constructor cannot be called!");
		}
	}

	JS_CLASS_METHOD_IMPL(WindowClass::onShow)
	{
		auto window = static_cast<DarkDescent::Window*>(args.This()->GetInternalField(0).As<v8::External>()->Value());
		window->show();
	}

	JS_CLASS_METHOD_IMPL(WindowClass::onMinimize)
	{
		auto window = static_cast<DarkDescent::Window*>(args.This()->GetInternalField(0).As<v8::External>()->Value());
		window->minimize();
	}

	JS_CLASS_METHOD_IMPL(WindowClass::onMaximize)
	{
		auto window = static_cast<DarkDescent::Window*>(args.This()->GetInternalField(0).As<v8::External>()->Value());
		window->maximize();
	}

	JS_CREATE_CLASS(WindowClass)
	{
		builder.setName("Window");
		builder.setConstructor(ctor);
		builder.setMethod("onLoad");
		builder.setMethod("minimize");
		builder.setMethod("maximize");
		builder.setMethod("unmaximize"); // ??
		builder.setMethod("show", onShow);
		builder.setMethod("close");
		builder.setMethod("onClose");
		builder.setMethod("onClosed");
		builder.setInternalFieldCount(1);
	}
}