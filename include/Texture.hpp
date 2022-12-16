#pragma once

#include "Resource.hpp"

namespace DarkDescent
{
	class Texture : public Resource
	{
		public:
			explicit Texture(const std::string& path);
			explicit Texture(std::string&& path);
	};
}