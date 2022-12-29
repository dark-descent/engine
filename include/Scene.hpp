#pragma once

#include "pch.hpp"
#include "Resource.hpp"

namespace DarkDescent
{
	struct SceneData
	{
		
	};

	class Scene: public Resource<Scene, SceneData>
	{
	public:
		Scene(const std::string& path): Resource(path) {}
		virtual ~Scene() {}

		virtual void onLoad() {  }
	};
}