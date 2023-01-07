#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "PersistentVector.hpp"
#include "Window.hpp"

namespace DarkDescent
{
	namespace JS { class Env; }
	class WindowManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(WindowManager),
			isRunning_(false),
			gameWindow_(nullptr)
		{ }

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	public:
		bool pumpEvents();
		std::size_t createWindow(const JS::Env& env, v8::Local<v8::Value> config, bool isGameWindow = false);
		inline Window& getWindow(std::size_t index) { return windows_.at(index); };
		inline Window& getWindowFromID(std::uint32_t id) { return windows_.at(idToIndexMap_.at(id)); };

		inline Window& getGameWindow() const
		{
			assert(gameWindow_ != nullptr);
			return *gameWindow_;
		}

		inline bool getGameWindow(Window** window) const
		{
			if (gameWindow_ != nullptr)
			{
				*window = gameWindow_;
				return true;
			}
			return false;
		}
	private:

		static int eventWatcher(void* data, SDL_Event* event);

		bool isRunning_;
		PersistentVector<Window, 2> windows_;
		std::unordered_map<std::uint32_t, std::size_t> idToIndexMap_;
		Window* gameWindow_;
	};
}