#include "Renderer.hpp"
#include "WindowManager.hpp"

namespace DarkDescent
{
	Renderer::Renderer(Window* window):
		window_(window)
	{
		assert(window_ != nullptr);
		context_ = SDL_GL_CreateContext(window->sdlWindow_);
	}

	void Renderer::render()
	{
		assert(window_ != nullptr);
		glViewport(0, 0, window_->width(), window_->height());
    	glClearColor(1.f, 0.1f, 0.1f, 1.f);
    	glClear(GL_COLOR_BUFFER_BIT);

    	SDL_GL_SwapWindow(window_->sdlWindow_);
	}
}