#pragma once

#include "Component.hpp"

namespace DarkDescent
{
	struct Transform
	{
		COMPONENT_DATA();

		float x;
		float y;
	};

	INITIALIZE_COMPONENT_DATA(Transform);
}