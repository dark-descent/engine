#include "Window.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	Window::Window(std::size_t index, Window::Config&& config):
		index_(index),
		sdlWindow_(SDL_CreateWindow(config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL)),
		screenSurface_(SDL_GetWindowSurface(sdlWindow_)),
		isDestroyed_(false),
		width_(config.width),
		height_(config.height)
	{
		if (sdlWindow_ == nullptr)
			throw TraceException("Could not create window!");
	}

	Window::~Window()
	{
		destroy();
	}

	void Window::show()
	{
		SDL_ShowWindow(sdlWindow_);
	}

	void Window::minimize()
	{
		SDL_MinimizeWindow(sdlWindow_);
	}

	void Window::maximize()
	{
		SDL_MaximizeWindow(sdlWindow_);
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