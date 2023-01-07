#pragma once

#include "Component.hpp"

namespace DarkDescent
{
	struct SpriteRenderer : public Component<SpriteRenderer>
	{
		SpriteRenderer(float x = 0.0f, float y = 0.0f) : Component(), x(x), y(y) {};

		float x;
		float y;

		std::string log()
		{
			return std::format("struct SpriteRenderer\n  x = {}\n  y = {}", x, y);
		}
	};
}