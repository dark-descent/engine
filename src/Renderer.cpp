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


	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

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

	void Renderer::renderSetup()
	{
		assert(window_ != nullptr);
		glViewport(0, 0, window_->width(), window_->height());
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::render()
	{
		assert(window_ != nullptr);
		SDL_GL_SwapWindow(window_->sdlWindow_);
	}
}