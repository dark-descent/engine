#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Resource
	{
	public:
		explicit Resource(std::string&& path);
		explicit Resource(const std::string& path);
		~Resource();

		bool isLoaded() const;

		bool load();

		const std::string path;
	
	private:
		bool isLoaded_;
	};
}