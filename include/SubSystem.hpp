#pragma once

#define SUB_SYSTEM_CTORS(CLASS_NAME) CLASS_NAME(const Engine& engine): SubSystem(engine) { } \
CLASS_NAME(const CLASS_NAME&) = delete; \
CLASS_NAME(CLASS_NAME&&) = delete; \
virtual ~CLASS_NAME() { } \
friend class Engine;


namespace DarkDescent
{
	class Engine;

	class SubSystem
	{
	protected:
		SubSystem(const Engine& engine);
		SubSystem(const SubSystem&) = delete;
		SubSystem(SubSystem&&) = delete;
		virtual ~SubSystem() = 0;

	protected:
		void initialize(const char* name);
		void terminate();

		virtual void onInitialize();
		virtual void onTerminate();
		virtual void run();

	private:
		std::string name_;
	
	protected:
		const Engine& engine_;

		friend class Engine;
	};

	template <typename T>
	concept IsSubSystem = std::is_base_of<SubSystem, T>::value;
}