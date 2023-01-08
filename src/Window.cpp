#include "Window.hpp"
#include "TraceException.hpp"
#include "js/Env.hpp"
#include "js/Helpers.hpp"

namespace DarkDescent
{
	Window::Config Window::Config::parse(const JS::Env& env, v8::Local<v8::Value> jsConfig, const char* defaultTitle)
	{
		Window::Config config{ .title = defaultTitle };

		auto titleVal = JS::getFromObject(env, jsConfig, "title");
		if (!titleVal.IsEmpty())
		{
			auto title = titleVal.ToLocalChecked();
			if (title->IsString())
			{
				JS::parseString(env, title, config.title);
			}
		}

		auto parseNumber = [ & ](const char* key, std::uint32_t& configProp)
		{
			auto val = JS::getFromObject(env, jsConfig, key);
			if (!val.IsEmpty())
				JS::parseNumber(env, val.ToLocalChecked(), configProp);
		};

		auto parseBool = [ & ](const char* key, bool& configProp)
		{
			auto val = JS::getFromObject(env, jsConfig, key);
			if (!val.IsEmpty())
				configProp = val.ToLocalChecked()->BooleanValue(env.isolate());
		};

		parseNumber("width", config.width);
		parseNumber("height", config.height);
		parseNumber("minWidth", config.minWidth);
		parseNumber("maxWidth", config.maxWidth);
		parseNumber("minHeight", config.minHeight);
		parseNumber("maxHeight", config.maxHeight);
		parseBool("resizable", config.resizable);
		parseBool("fullscreenable", config.fullscreenable);
		parseBool("minimizable", config.minimizable);
		parseBool("initialHidden", config.initialHidden);

		v8::Local<v8::Value> initialStateVal;
		if (JS::getFromObject(env, jsConfig, "initialState").ToLocal(&initialStateVal))
		{
			std::uint32_t val = 0;
			if (JS::parseNumber(env, initialStateVal, val))
			{
				config.initialState = static_cast<Window::State>(val);
			}
		}

		return config;
	}

	Window::Window(const JS::Env& env, std::size_t index, Window::Config&& config, bool isGameWindow):
		index_(index),
		sdlWindow_(SDL_CreateWindow(
			config.title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			config.width,
			config.height,
			(config.initialHidden ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN) | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
		)),
		screenSurface_(SDL_GetWindowSurface(sdlWindow_)),
		isInitiallyShown_(false),
		state_(config.initialState),
		isDestroyed_(false),
		width_(config.width),
		height_(config.height),
		jsWindow_(env),
		isGameWindow_(isGameWindow)
	{
		if (sdlWindow_ == nullptr)
			throw TraceException("Could not create window!");
		jsWindow_.reset(env.getClass<JS::WindowClass>().instantiate({ v8::External::New(env.isolate(), this) }).ToLocalChecked());
	}

	Window::~Window()
	{
		destroy();
	}

	void Window::show()
	{
		if (!isInitiallyShown_) [[unlikely]]
		{
			switch (state_)
			{
				case State::MAXIMIZED:
					maximize();
					break;
				case State::MINIMIZED:
					minimize();
					break;
				case State::UNMAXIMIZED:
					unmaximize();
				default:
					SDL_ShowWindow(sdlWindow_);
					break;
			}
			isInitiallyShown_ = true;
		}
		else [[likely]]
		{
			SDL_ShowWindow(sdlWindow_);
		}
	}

	void Window::minimize()
	{
		SDL_MinimizeWindow(sdlWindow_);
	}

	void Window::maximize()
	{
		SDL_MaximizeWindow(sdlWindow_);
	}

	void Window::unmaximize()
	{
		SDL_RestoreWindow(sdlWindow_);
	}

	void Window::destroy()
	{
		if (!isDestroyed_)
		{
			SDL_DestroyWindow(sdlWindow_);
			sdlWindow_ = nullptr;
			screenSurface_ = nullptr;
			isDestroyed_ = true;
		}
	}
}