#pragma once

#include "Shader.hpp"

namespace DarkDescent
{
	class Material
	{
	public:
		Material(const ShaderProgram& program):
			program(program),
			buffer(0),
			data_(static_cast<char*>(::malloc(program.attributesBufferSize)))
		{
			memset(data_, 0, program.attributesBufferSize);
			glGenBuffers(static_cast<GLsizei>(program.attributesBufferSize), &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, program.attributesBufferSize, data_, GL_DYNAMIC_DRAW);
		}

		template<typename T>
		void set(const char* name, T&& value)
		{
			const auto& param = program.getAttribute(name);
			T* ptr = static_cast<T*>(static_cast<void*>(&data_[param.offset]));
			ptr = value;
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferSubData(GL_ARRAY_BUFFER, param.offset, param.size, ptr);
		}

		template<typename T>
		const T& get(const char* name)
		{
			const auto& param = program.getAttribute(name);
			return *static_cast<T*>(static_cast<void*>(&data_[param.offset]));
		}

		const ShaderProgram& program;
		GLuint buffer;

	private:
		char* data_;

	};
}