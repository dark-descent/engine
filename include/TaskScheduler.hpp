#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

#include "Task.hpp"
#include "LockFreeQueue.hpp"
#include "TaskWaitList.hpp"

#define TASK(name) Task name(Task::Counter* __COROUTINE_COUNTER__, TaskScheduler* scheduler, const Engine& engine, void* arg)
#define TASK_AWAIT(counter) if(counter != nullptr) co_yield { counter, false };
#define TASK_RETURN co_yield { __COROUTINE_COUNTER__, true }

namespace DarkDescent
{
	class TaskScheduler;
	class Engine;

	using TaskFunction = Task(*)(Task::Counter* counter, TaskScheduler* scheduler, const Engine& engine, void* arg);

	struct TaskInfo
	{
		TaskFunction function = nullptr;
		void* arg = nullptr;

		TaskInfo(TaskFunction function, void* arg = nullptr): function(function), arg(arg) { }
	};

	class TaskScheduler: public SubSystem
	{
		SUB_SYSTEM_CTORS(TaskScheduler),
			maxTasks_(1024),
			readyQueue_(),
			mainThreadQueue_(),
			waitList_(),
			mainThreadWaitList_(),
			threads_(),
			mainThreadID_(std::this_thread::get_id()),
			threadsRunning_(),
			executionThreads_(1),
			threadIdMap_(),
			isRunning_(false)
		{
			threadsRunning_.store(false);
		}

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;
		bool runNextTask(TaskHandle& handleOut);
		bool runNextMainThreadTask(TaskHandle& handleOut);
		void threadEntry(size_t threadID);
		bool handleTaskYield(TaskHandle& handle);
		bool handleMainThreadTaskYield(TaskHandle& handle);

	public:
		Task::Counter* runTasks(TaskInfo* Tasks, size_t TasksCount, bool mainThreadOnly = false);
		Task::Counter* runTask(TaskInfo Tasks, bool mainThreadOnly = false);
		Task::Counter* runTask(TaskFunction func, bool mainThreadOnly = false);

		size_t getThreadID();

		void joinThreads();

		const bool isRunning();

		void execNext(TaskHandle& TaskHandle);

		template<typename LoopConditionCallback>
		void exec(LoopConditionCallback shouldLoop)
		{
			if (mainThreadID_ != std::this_thread::get_id())
				throw std::runtime_error("Cannot call TaskScheduler::exec() from another thread than the main thread!");

			if (isRunning_)
				throw std::runtime_error("TaskScheduler::exec() is already running!");

			isRunning_ = true;

			bool didStartThreads = threadsRunning_.load() == 0;
			bool didInitializeThreads = threads_.size() == 0;

			if (didInitializeThreads)
				initThreads();

			if (didStartThreads)
				runThreads();

			TaskHandle TaskHandle;

			while (shouldLoop())
			{
				if (runNextMainThreadTask(TaskHandle))
					handleMainThreadTaskYield(TaskHandle);

				if (shouldLoop() && runNextTask(TaskHandle))
					handleTaskYield(TaskHandle);
			}

			isRunning_ = true;

			if (didStartThreads)
				stopThreads();

			if (didInitializeThreads)
				joinThreads();
		}

		void initThreads();
		void runThreads();
		void stopThreads();
		void clearThreads();

	private:
		size_t maxTasks_;
		LockFree::Queue<TaskHandle>* readyQueue_;
		LockFree::Queue<TaskHandle>* mainThreadQueue_;
		TaskWaitList<TaskHandle> waitList_;
		TaskWaitList<TaskHandle> mainThreadWaitList_;

		std::vector<std::thread> threads_;
		std::thread::id mainThreadID_;
		std::atomic<int> threadsRunning_;
		size_t executionThreads_;

		std::unordered_map<std::thread::id, size_t> threadIdMap_;

		bool isRunning_;
	};
}