#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Scene
	{
	public:
		Scene(const std::string& path);
		~Scene();

		const std::string path;

		void load();
		void unload();
	};
}