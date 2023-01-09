#pragma once

#define SUB_SYSTEM_CTORS(CLASS_NAME) friend class Engine; \
CLASS_NAME(const CLASS_NAME&) = delete; \
CLASS_NAME(CLASS_NAME&&) = delete; \
virtual ~CLASS_NAME() { } \
CLASS_NAME(const char* name, const Engine& engine): SubSystem(name, engine)

#include "EventManager.hpp"
#include "Logger.hpp"

namespace DarkDescent
{
	class Engine;

	class SubSystem
	{
	public:
		struct Logger
		{
		public:
			Logger(const char* name);
			Logger(const Logger&) = delete;
			Logger(Logger&&) = delete;
			~Logger();

			template<typename... Args>
			void info(Args&&... args) const { logger_.info(prefix_, std::forward<Args>(args)...); }

			template<typename... Args>
			void warn(Args&&... args) const { logger_.warn(prefix_, std::forward<Args>(args)...); }

			template<typename... Args>
			void error(Args&&... args) const { logger_.error(prefix_, std::forward<Args>(args)...); }

			template<typename... Args>
			void exception(Args&&... args) const { logger_.exception(prefix_, std::forward<Args>(args)...); }
			
#ifdef _DEBUG
			template<typename... Args>
			void debug(Args&&... args) const { logger_.debug(prefix_, std::forward<Args>(args)...); }
#else
			template<typename... Args>
			void debug(Args&&... args) const {  }
#endif

		private:
			const std::string prefix_;
			const DarkDescent::Logger& logger_;
		};

	protected:
		SubSystem(const char* name, const Engine& engine);
		SubSystem(const SubSystem&) = delete;
		SubSystem(SubSystem&&) = delete;
		virtual ~SubSystem() = 0;

	private:
		void initialize();
		void allInitialized();
		void terminate();

	protected:
		virtual void onInitialize();
		virtual void onReady();
		virtual void onTerminate();



		void emitEvent(const char* name, void* data = nullptr);
		void addEventHandler(const char* name, EventHandler eventHandler, void* data = nullptr);
		void removeEventHandler(const char* name, EventHandler eventHandler);

		void emitEvent(Hash name, void* data = nullptr);
		void addEventHandler(Hash name, EventHandler eventHandler, void* data = nullptr);
		void removeEventHandler(Hash name, EventHandler eventHandler);

		template<typename T>
		void emitEvent(T event, void* data = nullptr)
		{
			emitEvent(static_cast<Hash>(event), data);
		}

		template<typename T>
		void addEventHandler(T event, EventHandler eventHandler, void* data = nullptr)
		{
			addEventHandler(static_cast<Hash>(event), eventHandler, data);
		}
		
	public:
		inline const Engine& engine() { return engine_; }

	protected:
		const std::string name;

	public:
		const Logger logger;

	protected:
		const Engine& engine_;

		friend class Engine;
	};

	template <typename T>
	concept IsSubSystem = std::is_base_of<SubSystem, T>::value;
}