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
	
		char* data() const { assert(isLoaded()); return data_; }

	private:
		bool isLoaded_;
		char* data_;
	};
}