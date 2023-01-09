#include "Renderer.hpp"
#include "WindowManager.hpp"
#include "TraceException.hpp"
#include "RenderSystem.hpp"
#include "ResourceManager.hpp"
#include "Engine.hpp"
#include "Shader.hpp"

namespace DarkDescent
{
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	Renderer::Renderer(RenderSystem& system, Window* window):
		window_(window),
		system_(system),
		resourceManager_(*system.engine().getSubSystem<ResourceManager>()),
		defaultMaterial_(nullptr)
	{
		assert(window_ != nullptr);
		context_ = SDL_GL_CreateContext(window->sdlWindow_);

		SDL_GL_MakeCurrent(window->sdlWindow_, context_);

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
			throw TraceException("Could not initialize glew!");
	}

	const Shader& Renderer::registerShader(const char* path, Shader::Type type)
	{
		system_.logger.info("registered ", type == Shader::Type::FRAGMENT ? "fragment" : "vertex", " shader (", path, ")");
		if (!shaders_.contains(path))
			shaders_.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple(resourceManager_.loadResourceImmediate<ShaderSource>(path), type));
		return getShader(path);
	}

	const ShaderProgram& Renderer::registerShaderProgram(const char* name, const char* vertex, const char* fragment)
	{
		return registerShaderProgram(name, shaders_.at(vertex), shaders_.at(fragment));
	}

	const ShaderProgram& Renderer::registerShaderProgram(const char* name, const Shader& vertex, const Shader& fragment)
	{
		system_.logger.info("registered ShaderProgram ", name);
		if (!shaderPrograms_.contains(name))
			shaderPrograms_.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(vertex, fragment));
		return getShaderProgram(name);
	}

	Material* createMaterial()
	{
		static std::size_t idCounter = 0;
		materials_.emplace();
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