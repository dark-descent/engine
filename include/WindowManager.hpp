#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

namespace DarkDescent
{
	struct Window
	{
		SDL_Window* window;
		SDL_Surface* screenSurface;
	};

	class WindowManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(WindowManager);

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;

		void enterEventLoop();

	private:
		void createWindow(const std::string& title, int width = 640, int height = 480);
		
		bool isRunning_ = false;
		std::vector<Window> windows_;
	};
}