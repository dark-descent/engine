#include "pch.hpp"
#include "js/Game.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"
#include "RenderSystem.hpp"

namespace DarkDescent::JS
{
	Game::Game(const Env& env) : ObjectWrapper(env) { }

	Game::~Game() { }

	void Game::initializeProps()
	{
		loadMethod(onInitialize_, "onInitialize");
		loadMethod(onLoad_, "onLoad");
		loadMethod(onTerminate_, "onTerminate");
	}

	JS_METHOD_IMPL(Game::onInitialize);
	JS_METHOD_IMPL(Game::onLoad);
	JS_METHOD_IMPL(Game::onTerminate);

	JS_CLASS_METHOD_IMPL(GameClass::ctor)
	{
		if(args.Length() >= 1 && args[0]->IsExternal()) [[likely]]
		{
			args.This()->SetInternalField(0, args[0]);
		}
		else [[unlikely]]
		{
			env.throwException("Cannot create engine by calling its constructor!");
		}
	}

	JS_CREATE_CLASS(GameClass)
	{
		builder.setName("Game");
		builder.setConstructor(ctor);
		builder.setMethod("onInitialize");
		builder.setMethod("onLoad");
		builder.setMethod("onTerminate");
		builder.set("window", v8::Undefined(env.isolate()));
		builder.setInternalFieldCount(1);
	}
}