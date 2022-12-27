#pragma once

namespace DarkDescent::JS
{
	class Env;
	class Object;
	
	namespace Process
	{
		void expose(const Env& env, Object& global);
	}
}