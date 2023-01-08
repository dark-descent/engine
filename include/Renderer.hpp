#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Window;

	class Renderer
	{
	public:
		Renderer(Window* window);

		void renderSetup();
		void render();

	private:
		Window* window_;
		SDL_Renderer* sdlRenderer_;
		SDL_GLContext context_;

		GLuint vertexArrayID_;
		GLuint vertexbuffer_;
	};
}