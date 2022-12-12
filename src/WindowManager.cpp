#include "WindowManager.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	void WindowManager::onInitialize()
	{
		SDL_Window* window = NULL;
		SDL_Surface* screenSurface = NULL;

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw TraceException(SDL_GetError());
	}

	void WindowManager::onTerminate()
	{
		for (auto& w : windows_)
			SDL_DestroyWindow(w.window);

		windows_.clear();

		SDL_Quit();
	}

	void WindowManager::createWindow(const std::string& title, int width, int height)
	{
		auto& w = windows_.emplace_back();
		w.window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (w.window == nullptr)
			throw TraceException(SDL_GetError());

		w.screenSurface = SDL_GetWindowSurface(w.window);

		SDL_FillRect(w.screenSurface, NULL, SDL_MapRGB(w.screenSurface->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(w.window);
	}

	void WindowManager::enterEventLoop()
	{
		assert(!isRunning_);
		isRunning_ = true;
		SDL_Event e;
		while (isRunning_)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					isRunning_ = false;
			}
		}
	}
}