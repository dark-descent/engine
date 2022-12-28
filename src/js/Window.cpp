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
		loadMethod(onLoad_, "onLoad");
		loadMethod(onClose_, "onClose");
		loadMethod(onClosed_, "onClosed");
	}

	JS_METHOD_IMPL(Window::onLoad);
	JS_METHOD_IMPL(Window::onClose);
	JS_METHOD_IMPL(Window::onClosed);

	JS_CLASS_METHOD_IMPL(WindowClass::ctor)
	{
		auto wm = Engine::getInstance().getSubSystem<WindowManager>();
		auto windoIndex = wm->createWindow("test");
		args.This()->SetInternalField(0, v8::External::New(env.isolate(), std::addressof(wm->getWindow(windoIndex))));
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
		// builder.setStaticMethod("create", onCreate); // TODO: async 
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