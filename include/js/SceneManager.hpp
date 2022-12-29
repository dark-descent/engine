#pragma once

#include "pch.hpp"
#include "js/Object.hpp"

namespace DarkDescent
{
	class SceneManager;
	
	namespace JS
	{
		class Env;

		namespace SceneManager
		{
			v8::Local<v8::Object> create(const Env& env, DarkDescent::SceneManager* system);
		}
	};
};
