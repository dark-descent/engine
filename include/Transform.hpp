#pragma once

#include "pch.hpp"
#include "Component.hpp"

namespace DarkDescent
{
	struct Transform : public Component<Transform>
	{
		Transform(float x = 0.0f, float y = 0.0f) : Component(), x(x), y(y) {};

		float x;
		float y;

		std::string log()
		{
			return std::format("struct Transform\n  x = {}\n  y = {}", x, y);
		}
	};
}