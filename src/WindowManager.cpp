#include "WindowManager.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"
#include "Engine.hpp"
#include "ScriptManager.hpp"
#include "js/Window.hpp"
#include "js/Env.hpp"

namespace DarkDescent
{
	void WindowManager::onInitialize()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw TraceException(SDL_GetError());

		SDL_AddEventWatch(eventWatcher, this);

		addEventHandler(ScriptManager::Events::ENV_CREATED, [](SubSystem* self, const Event& e, void* data)
		{
			const JS::Env& env = *static_cast<const JS::Env*>(e.data);
		env.registerClass<JS::WindowClass>();
		}, this);
	}

	void WindowManager::onReady()
	{
		isRunning_ = true;
	}

	void WindowManager::onTerminate()
	{
		windows_.foreach([](Window& window, std::size_t index) { window.destroy(); });

		windows_.clear();

		SDL_Quit();
	}

	int WindowManager::eventWatcher(void* data, SDL_Event* event)
	{
		if (event->type == SDL_WINDOWEVENT)
		{
			switch (event->window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
				{
					WindowManager& wm = *static_cast<WindowManager*>(data);
					Window& w = wm.getWindowFromID(event->window.windowID);
					w.width_ = event->window.data1;
					w.height_ = event->window.data2;
					wm.emitEvent(Hasher::hash("WINDOW_RESIZED"), std::addressof(w));
				}
				break;
			}

		}
		return 0;
	}

	std::size_t WindowManager::createWindow(const JS::Env& env, v8::Local<v8::Value> config, bool isGameWindow)
	{
		std::size_t index = windows_.getNextIndex();
		Window& w = windows_.emplace_back(env, index, Window::Config::parse(env, config, engine_.config().name.c_str()), isGameWindow);
		idToIndexMap_.insert({ SDL_GetWindowID(w.sdlWindow_), index });
		emitEvent(Hasher::hash("WINDOW_CREATED"), &index);
		if (isGameWindow)
		{
			assert(gameWindow_ == nullptr);
			gameWindow_ = std::addressof(w);
		}
		return index;
	}

	bool WindowManager::pumpEvents()
	{
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
				{
					isRunning_ = false;
				}
				break;
				case SDL_WINDOWEVENT:
				{
					switch (e.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
						{
							// emitEvent(Hasher::hash("WINDOW_DESTROY"), std::addressof(windows_.at(e.window.windowID)));
						}
						break;
					}
				}
				break;
			}
		}
		if (!isRunning_)
		{
			puts("running_ set to false");
		}
		return isRunning_;
	}
}