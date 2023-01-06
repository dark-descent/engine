#pragma once

#include "pch.hpp"
#include "js/Window.hpp"

namespace DarkDescent
{
	namespace JS { class Env; }
	class Window
	{
	public:
		enum class State
		{
			MINIMIZED = 0,
			MAXIMIZED = 1,
			UNMAXIMIZED = 2
		};

		struct Config
		{
			std::string title = "";
			bool resizable = true;
			bool fullscreenable = false;
			bool minimizable = true;
			std::uint32_t width = 640;
			std::uint32_t height = 480;
			std::uint32_t minWidth = 0;
			std::uint32_t maxWidth = 0;
			std::uint32_t minHeight = 0;
			std::uint32_t maxHeight = 0;
			State initialState = State::MAXIMIZED;
			bool initialHidden = true;

			static Window::Config parse(const JS::Env& env, v8::Local<v8::Value> config, const char* defaultTitle);
		};

		Window(const JS::Env& env, std::size_t index, Config&& config);
		~Window();

		void show();
		void minimize();
		void maximize();
		void unmaximize();
		void destroy();

		inline bool isDestroyed() const { return isDestroyed_; }

		inline std::uint32_t width() const { return width_; };
		inline std::uint32_t height() const { return height_; };

		inline v8::Local<v8::Value> jsObject() const { return jsWindow_.value(); };


	private:
		std::size_t index_;
		SDL_Window* sdlWindow_;
		SDL_Surface* screenSurface_;

		bool isInitiallyShown_;
		State state_;

		std::uint32_t width_;
		std::uint32_t height_;

		bool isDestroyed_;
		JS::Window jsWindow_;

		friend class Renderer;
		friend class WindowManager;
	};
}