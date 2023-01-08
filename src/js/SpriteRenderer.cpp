#include "js/SpriteRenderer.hpp"
#include "SpriteRenderer.hpp"
#include "GameObject.hpp"

namespace DarkDescent::JS
{
	JS_CLASS_METHOD_IMPL(SpriteRendererClass::ctor)
	{
		printf("SpriteRendererClass::ctor index %zu bitmask %zu\n", SpriteRenderer::index, SpriteRenderer::bitmask);
		if (args.Length() >= 1 && args[0]->IsObject())
		{
			auto go = static_cast<GameObject*>(args[0].As<v8::Object>()->GetInternalField(0).As<v8::External>()->Value());
			auto t = go->addComponent<SpriteRenderer>();
			Logger::get().debug("Added component ", t->log());
		}
		else
		{

		}
	}

	JS_CREATE_CLASS(SpriteRendererClass)
	{
		builder.setName("Transform");
		builder.setConstructor(ctor);
	}
}