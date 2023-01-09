#pragma once

#include "pch.hpp"
#include "TextResource.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	using ShaderSource = TextResource;

	struct Shader
	{
		enum class Type: int
		{
			VERTEX = GL_VERTEX_SHADER,
			FRAGMENT = GL_FRAGMENT_SHADER,
		};

		Shader(ShaderSource& source, Type type);
		~Shader();

		inline GLuint shader() const noexcept { return shader_; }

	private:
		GLuint shader_;
	};

	struct ShaderProgram
	{
	public:
		struct Param
		{
			static constexpr std::size_t NAME_LENGTH = 32;

			std::array<char, NAME_LENGTH> name = { 0 };
			std::size_t size = 0;
			std::size_t offset = 0;
			GLenum type = 0;
		};

	private:
		static GLuint create(const Shader& vertex, const Shader& fragment);
		static std::vector<ShaderProgram::Param> getParams(GLuint program, GLint paramType);
		static std::size_t calculateAttributeBufferSize(const std::vector<Param>& param);

	public:
		ShaderProgram(const Shader& vertex, const Shader& fragment);
		~ShaderProgram();

		inline void use() const noexcept { glUseProgram(program); }

		const Param& getAttribute(const char* name)
		{
			for(const auto& attr : attributes)
			{
				if(strcmp(attr.name.data(), name) == 0)
				{
					return attr;
				}
			}
			throw TraceException("Could not get attribute offset!");
		}

		const GLuint program;
		const std::vector<Param> attributes;
		const std::vector<Param> uniforms;
		const std::size_t attributesBufferSize;
	};
}