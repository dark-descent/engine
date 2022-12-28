#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Window
	{
	public:
		struct Config
		{
			std::string title = "";
			bool resizable = true;
			bool fullscreen = false;
			std::uint32_t width = 640;
			std::uint32_t height = 480;
		};

		Window(std::size_t index, Config&& config);
		~Window();

		void show();
		void minimize();
		void maximize();
		void destroy();

		inline bool isDestroyed() const { return isDestroyed_; }

		inline std::uint32_t width() const { return width_; };
		inline std::uint32_t height() const { return height_; };


	private:
		std::size_t index_;
		SDL_Window* sdlWindow_;
		SDL_Surface* screenSurface_;

		std::uint32_t width_;
		std::uint32_t height_;

		bool isDestroyed_;

		friend class Renderer;
		friend class WindowManager;
	};
}