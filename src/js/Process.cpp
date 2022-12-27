#include "js/Process.hpp"
#include "js/Env.hpp"
#include "js/Object.hpp"
#include "js/Helpers.hpp"
#include "Engine.hpp"

namespace DarkDescent::JS::Process
{
	void expose(const Env& env, Object& global)
	{
		auto process = JS::Object(env);
		
		(*process)->SetAccessor(env.context(), JS::string(env, "args"), [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
		{
			const Env& env = Env::fromIsolate(info.GetIsolate());
			
			v8::Local<v8::Array> argsArray = v8::Array::New(env.isolate());

			std::uint32_t i = 0;

			for (const auto arg : Engine::getInstance().gameArgs)
				argsArray->Set(env.context(), i++, JS::string(env, arg));


			info.GetReturnValue().Set(argsArray);
		});

		global.set("process", *process);
	}
}