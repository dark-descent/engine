#pragma once

#include "pch.hpp"
#include "Resource.hpp"

namespace DarkDescent
{
	class Script: public Resource<Script, std::string>
	{
	public:
		Script(const std::string& path): Resource(path) { };
		virtual ~Script() { delete data_; };

		virtual void onLoad()
		{
			std::ifstream is(path);
			data_ = new std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		}
	};
}