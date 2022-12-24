#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Window;

	class Renderer
	{
	public:
		Renderer(Window* window);

		void render();

	private:
		Window* window_;
		SDL_Renderer* sdlRenderer_;
		SDL_GLContext context_;
	};
}