#include "pch.hpp"
#include "Logger.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	DarkDescent::Logger& logger = DarkDescent::Logger::get();

	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		logger.error("Error initializing SDL: ", SDL_GetError());
		return 1;
	}

	SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

	logger.info("Hello, World!");

	bool isRunning = true;

	SDL_Event event;

	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					isRunning = false;
					break;
			}
		}
	}

	SDL_Quit();

	DarkDescent::Logger::terminate();

	return 0;
}