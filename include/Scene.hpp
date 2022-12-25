#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Resource;
	class Scene
	{
	public:
		Scene(const std::string& path, const Resource& resource);
		~Scene();

		const std::string path;
		const Resource& resource;

		void load();
		void unload();
	};
}