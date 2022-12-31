#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

#include "Task.hpp"
#include "LockFreeQueue.hpp"
#include "PersistentVector.hpp"
#include "Logger.hpp"
#include "TaskWaitList.hpp"

namespace DarkDescent
{
	class TaskScheduler: public SubSystem
	{
		struct Awaiter: public std::suspend_always
		{
			void await_suspend(std::coroutine_handle<Task::promise_type> handle)
			{
				handle.promise().setCounter(counter_);
			}

			Awaiter(Task::Counter* counter): std::suspend_always(),
				counter_(counter)
			{ }

		private:
			Task::Counter* counter_;
		};

		SUB_SYSTEM_CTORS(TaskScheduler),
			tasksQueue_(1024),
			atomicCounters_(),
			waitList_()
		{ }

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	public:
		void executeNext()
		{
			Task::Handle handle;
			if (tasksQueue_.try_pop(handle))
			{
				if (handle)
				{
					handle.resume();
					auto& promise = handle.promise();
					if (promise.isWaiting())
						waitList_.push(handle);
					else if (!promise.isDone())
						tasksQueue_.push(handle);
					else
					{
						waitList_.findAndRelease([ & ](Task::Handle& handle)
						{
							if (!handle.promise().isWaiting())
							{
								tasksQueue_.push(handle);
								return true;
							}
						return false;
						});
					}
				}
			}
		}

		inline bool hasPendingTasks() const { return tasksQueue_.size() > 0; }

		Task::Counter* allocCounter(std::uint32_t count)
		{
			return std::addressof(atomicCounters_.emplace_back(count));
		}

		Task::Counter* allocCounter(std::size_t count)
		{
			return allocCounter(static_cast<std::uint32_t>(count));
		}

		void runTask(Task::Info::Function task, void* arg = nullptr)
		{
			tasksQueue_.push(task(*this, nullptr));
		}

		void runTask(const Task::Info& task)
		{
			tasksQueue_.push(task.function(*this, task.arg));
		}

		void runTasks(const Task::Info& task, std::size_t count)
		{
			for (std::size_t i = 0; i < count; i++)
				tasksQueue_.push(task.function(*this, task.arg));
		}

		void runTasks(const std::vector<Task::Info>& tasks)
		{
			for (auto& task : tasks)
				tasksQueue_.push(task.function(*this, task.arg));
		}

		Awaiter awaitTask(Task::Info::Function task, void* arg = nullptr)
		{
			std::size_t size = 1;
			Task::Counter* counter = allocCounter(size);
			Task::Handle h = task(*this, arg);
			h.promise().setDependencyCounter(counter);
			tasksQueue_.push(std::move(h));
			return { counter };
		}

		Awaiter awaitTasks(const Task::Info& task, std::size_t count)
		{
			Task::Counter* counter = allocCounter(count);

			for (std::size_t i = 0; i < count; i++)
			{
				Task::Handle h = task.function(*this, task.arg);
				h.promise().setDependencyCounter(counter);
				tasksQueue_.push(std::move(h));
			}
			return { counter };
		}

		Awaiter awaitTasks(const std::vector<Task::Info>& tasks)
		{
			Task::Counter* counter = allocCounter(tasks.size());

			for (auto& task : tasks)
			{
				Task::Handle h = task.function(*this, task.arg);
				h.promise().setDependencyCounter(counter);
				tasksQueue_.push(std::move(h));
			}

			return { counter };
		}

	private:
		LockFree::Queue<Task::Handle> tasksQueue_;
		PersistentVector<Task::Counter, 1024> atomicCounters_;
		TaskWaitList<Task::Handle> waitList_;
	};
}