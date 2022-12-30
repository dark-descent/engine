#include "pch.hpp"
#include "js/Game.hpp"
#include "js/Env.hpp"
#include "Engine.hpp"

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
		
	}

	JS_CLASS_METHOD_IMPL(GameClass::onInitialize)
	{
		
	}

	JS_CLASS_METHOD_IMPL(GameClass::onLoad)
	{
		
	}

	JS_CLASS_METHOD_IMPL(GameClass::onTerminate)
	{
		
	}

	JS_CREATE_CLASS(GameClass)
	{
		builder.setName("Game");
		builder.setConstructor(ctor);
		builder.setMethod("onInitialize");
		builder.setMethod("onLoad");
		builder.setMethod("onTerminate");
		builder.setInternalFieldCount(1);
	}
}