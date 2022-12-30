#pragma once

#include "pch.hpp"
#include "Resource.hpp"
#include "js/Scene.hpp"

namespace DarkDescent
{
	class Scene
	{
	public:
		Scene(const std::string& name, const JS::Env& env, v8::Local<v8::Value> jsScene);

		virtual ~Scene();

		virtual void onLoad();
		virtual void onUnload(); 

		const JS::Scene& jsScene() const { return jsScene_; }

		const std::string name;

	private:
		JS::Scene jsScene_;
	};
}