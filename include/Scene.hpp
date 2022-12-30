#pragma once

#include "pch.hpp"
#include "Resource.hpp"
#include "js/Scene.hpp"

namespace DarkDescent
{
	class ArchManager;
	class GameObjectManager;
	
	class Scene
	{
	public:
		Scene(const std::string& name, const JS::Env& env, v8::Local<v8::Value> jsScene);

		virtual ~Scene();

		virtual void onLoad(std::uint8_t archIndex);
		virtual void onUnload(); 

		void spawn();

		const JS::Scene& jsScene() const { return jsScene_; }

		const std::string name;

	private:
		ArchManager& archManager_;
		GameObjectManager& gameObjectManager_;
		JS::Scene jsScene_;
		std::uint8_t archIndex_;
	};
}