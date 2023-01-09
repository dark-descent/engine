#pragma once

#include "pch.hpp"
#include "Shader.hpp"
#include "Material.hpp"

namespace DarkDescent
{
	class Window;
	class RenderSystem;

	class Renderer
	{
	public:
		Renderer(RenderSystem& renderSystem, Window* window);

		void renderSetup();
		void render();

		const Shader& registerShader(const char* path, Shader::Type type);
		
		inline const Shader& getShader(const char* path) const { return shaders_.at(path); }
		inline const ShaderProgram& getShaderProgram(const char* name) const { return shaderPrograms_.at(name); }

		const ShaderProgram& registerShaderProgram(const char* name, const char* vertex, const char* fragment);
		const ShaderProgram& registerShaderProgram(const char* name, const Shader& vertex, const Shader& fragment);

		Material* createMaterial(const char* shaderProgramName);

		inline const ShaderProgram& getMaterial(const char* name) const
		{
			return shaderPrograms_.at(name);
		}

		void setDefaultMaterial(Material* material)
		{
			assert(material != nullptr);
			defaultMaterial_ = material;
		}

	private:
		Window* window_;
		RenderSystem& system_;
		ResourceManager& resourceManager_;
		SDL_Renderer* sdlRenderer_;
		SDL_GLContext context_;

		GLuint vertexArrayID_;
		GLuint vertexbuffer_;

		std::unordered_map<const char*, Shader> shaders_;
		std::unordered_map<const char*, ShaderProgram> shaderPrograms_;
		std::unordered_map<const char*, Material> materials_;
		Material* defaultMaterial_ = nullptr;
	};
}