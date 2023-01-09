#include "Shader.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	Shader::Shader(ShaderSource& source, Type type):
		shader_(glCreateShader(static_cast<GLenum>(type)))
	{
		const char* shaderSource = source.data().c_str();
		glShaderSource(shader_, 1, &shaderSource, NULL);
		glCompileShader(shader_);
		int success = 0;
		char infoLog[512];
		glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader_, 512, NULL, infoLog);
			throw TraceException(infoLog);
		}
	}

	Shader::~Shader()
	{
		glDeleteShader(shader_);
	}

	std::vector<ShaderProgram::Param> ShaderProgram::getParams(GLuint program, GLint paramType)
	{
		GLint count = 0;
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
		std::vector<ShaderProgram::Param> params(count);
		std::size_t offset = 0;
		for (GLint i = 0; i < count; i++)
		{
			auto& param = params.at(i);
			GLint nameLength = 0;
			GLint size = 0;
			glGetActiveAttrib(program, static_cast<GLuint>(i), Param::NAME_LENGTH, &nameLength, &size, &param.type, param.name.data());
			param.name.at(nameLength) = '\0';
			param.size = static_cast<std::size_t>(size);
			param.offset = offset;
			offset += param.size;
		}
		return params;
	}

	GLuint ShaderProgram::create(const Shader& vertex, const Shader& fragment)
	{
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex.shader());
		glAttachShader(program, fragment.shader());
		glLinkProgram(program);

		GLint success = 0;
		GLchar infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			throw TraceException(infoLog);
		}

		return program;
	}

	std::size_t ShaderProgram::calculateAttributeBufferSize(const std::vector<ShaderProgram::Param>& params)
	{
		std::size_t attributesBufferSize = 0;
		for (const auto& attr : params)
			attributesBufferSize += attr.size;
		return 	attributesBufferSize;
	}

	ShaderProgram::ShaderProgram(const Shader& vertex, const Shader& fragment):
		program(ShaderProgram::create(vertex, fragment)),
		attributes(getParams(program, GL_ACTIVE_ATTRIBUTES)),
		uniforms(getParams(program, GL_ACTIVE_UNIFORMS)),
		attributesBufferSize(calculateAttributeBufferSize(attributes))
	{ }

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(program);
	}
}