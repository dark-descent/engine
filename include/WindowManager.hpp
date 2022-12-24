#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "PersistentVector.hpp"
#include "Window.hpp"

namespace DarkDescent
{
	class WindowManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(WindowManager) { };

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;

		void enterEventLoop();
	
	public:
		std::size_t createWindow(const std::string& title, int width = 640, int height = 480);
		inline Window& getWindow(std::size_t index) { return windows_.at(index); };
		inline Window& getWindowFromID(std::uint32_t id) { return windows_.at(idToIndexMap_.at(id)); };
	private:

		static int eventWatcher(void* data, SDL_Event* event);

		bool isRunning_ = false;
		PersistentVector<Window, 2> windows_;
		std::unordered_map<std::uint32_t, std::size_t> idToIndexMap_;
	};
}