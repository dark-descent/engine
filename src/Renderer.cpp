#include "Renderer.hpp"
#include "WindowManager.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	Renderer::Renderer(Window* window):
		window_(window)
	{
		assert(window_ != nullptr);
		context_ = SDL_GL_CreateContext(window->sdlWindow_);

		SDL_GL_MakeCurrent(window->sdlWindow_, context_);

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
			throw TraceException("Could not initialize glew!");


		glGenVertexArrays(1, &vertexArrayID_);
		glBindVertexArray(vertexArrayID_);


		glGenBuffers(1, &vertexbuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	}

	void Renderer::render()
	{
		assert(window_ != nullptr);
		glViewport(0, 0, window_->width(), window_->height());
		// glClearColor(1.f, 0.1f, 0.1f, 1.f);
		// glClear(GL_COLOR_BUFFER_BIT);
		glClear( GL_COLOR_BUFFER_BIT );

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window_->sdlWindow_);
	}
}