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
		SubSystem(const Engine& engine): engine_(engine) { }
		SubSystem(const SubSystem&) = delete;
		SubSystem(SubSystem&&) = delete;
		virtual ~SubSystem() = 0 { }

	protected:
		virtual void initialize() { }
		virtual void terminate() { }
		virtual void run() { }

		const Engine& engine_;
	};
}