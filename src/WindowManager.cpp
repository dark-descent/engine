#include "WindowManager.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"
#include "Engine.hpp"
#include "ScriptManager.hpp"
#include "js/Window.hpp"

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
			env.global().set("Window", env.registerClass<JS::WindowClass>());
		}, this);
	}

	void WindowManager::onReady()
	{

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

	std::size_t WindowManager::createWindow(const std::string& title, int width, int height)
	{
		std::size_t index = windows_.getNextIndex();
		Window& w = windows_.emplace_back(index, Window::Config{ .title = title });
		idToIndexMap_.insert({ SDL_GetWindowID(w.sdlWindow_), index });
		emitEvent(Hasher::hash("WINDOW_CREATED"), &index);
		return index;
	}

	void WindowManager::enterEventLoop()
	{
		assert(!isRunning_);
		isRunning_ = true;
		SDL_Event e;
		while (isRunning_)
		{
			SDL_WaitEventTimeout(&e, 15);

			do
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
			} while (SDL_PollEvent(&e));

			windows_.foreach([ & ](Window& window, std::size_t index)
			{
				if (!window.isDestroyed())
					emitEvent(Hasher::hash("RENDER"), &index);
			});
		}
	}
}