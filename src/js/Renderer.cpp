#include "js/Renderer.hpp"
#include "js/Env.hpp"
#include "js/Object.hpp"
#include "js/Helpers.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "RenderSystem.hpp"
#include "js/Material.hpp"

namespace DarkDescent::JS::Renderer
{
	using GameRenderer = DarkDescent::Renderer;

	static GameRenderer& getRenderer(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		RenderSystem* system = JS::parseExternalData<RenderSystem>(args);
		GameRenderer* renderer = system->gameRenderer();
		assert(renderer != nullptr);
		return *renderer;
	}

	JS_CALLBACK(registerShader)
	{
		if (args.Length() >= 3)
		{
			if (args[0]->IsString() && args[1]->IsString() && args[2]->IsString())
			{
				std::string name = JS::parseString(env, args[0]);
				std::string vertexPath = JS::parseString(env, args[1]);
				std::string fragmentPath = JS::parseString(env, args[2]);

				GameRenderer& renderer = getRenderer(args);

				auto& vertex = renderer.registerShader(vertexPath.c_str(), Shader::Type::VERTEX);
				auto& fragment = renderer.registerShader(fragmentPath.c_str(), Shader::Type::FRAGMENT);
				renderer.registerShaderProgram(name.c_str(), vertex, fragment);
			}
		}
	}


	JS_CALLBACK(createMaterial)
	{
		if (args.Length() >= 2)
		{
			args.GetReturnValue().Set(env.getClass<MaterialClass>().instantiate({ args[0], args[1] }).ToLocalChecked());
		}
		else
		{
			env.throwException("Could not create material! Not enough args!");
		}
	}

	JS_CALLBACK(setDefaultMaterial)
	{
		if (args.Length() >= 1 && args[0]->IsObject())
		{
			GameRenderer& renderer = getRenderer(args);
			renderer.setDefaultMaterial(JS::getInternalPointer<Material>(args[0], 0));
		}
		else
		{
			env.throwException("Could not create material! Not enough args!");
		}
	}

	v8::Local<v8::Object> createNamespace(const JS::Env& env)
	{
		auto* renderSystem = Engine::getInstance().getSubSystem<RenderSystem>();
		assert(renderSystem != nullptr);
		JS::Object renderer(env);
		renderer.set("registerShader", registerShader, renderSystem, v8::PropertyAttribute::ReadOnly);
		renderer.set("createMaterial", createMaterial, renderSystem, v8::PropertyAttribute::ReadOnly);
		renderer.set("setDefaultMaterial", setDefaultMaterial, renderSystem, v8::PropertyAttribute::ReadOnly);
		return *renderer;
	}

}