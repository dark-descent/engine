#pragma once

#include "pch.hpp"
#include "Resource.hpp"
#include "js/Scene.hpp"

namespace DarkDescent
{
	class ArchManager;
	class GameObjectManager;
	class GameObject;
	
	class Scene
	{
	public:
		Scene(const std::string& name, const JS::Env& env, v8::Local<v8::Value> jsScene);

		virtual ~Scene();

		virtual void onLoad(std::uint8_t archMapIndex);
		virtual void onUnload(); 

		GameObject* spawn();

		const JS::Scene& jsScene() const { return jsScene_; }
		std::uint8_t archMapIndex() const { return archMapIndex_; }

		const std::string name;

	private:
		ArchManager& archManager_;
		GameObjectManager& gameObjectManager_;
		JS::Scene jsScene_;
		std::uint8_t archMapIndex_;
	};
}