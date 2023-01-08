#pragma once

#include "Component.hpp"
#include "pch.hpp"

namespace DarkDescent
{
	struct SpriteRenderer: public Component<SpriteRenderer>
	{
		static const float vertices[9];

		SpriteRenderer():
			Component(),
			vbo()
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}

		std::string log()
		{
			return std::format("struct SpriteRenderer");
		}

		std::uint32_t vbo;
	};
}