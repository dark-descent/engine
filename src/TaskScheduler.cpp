#include "TaskScheduler.hpp"
#include "Logger.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	void TaskScheduler::onInitialize()
	{
		queue_.emplace(1024 * 8);
		isRunning_.store(true, std::memory_order::release);
		for(std::size_t i = 0; i < 4; i++)
		{
			workerThreads_.emplace_back([&]()
			{
				auto& queue = queue_.value();
				while(isRunning_.load(std::memory_order::acquire))
				{
					if(!queue.isDone())
						runNext(queue);
				}
			});
		}
	}

	void TaskScheduler::onReady()
	{

	}

	void TaskScheduler::onTerminate()
	{
		isRunning_.store(false, std::memory_order::release);
		for(auto& thread : workerThreads_)
			thread.join();
	}

	void TaskScheduler::schedule(Queue& queue, TaskPtr taskPtr)
	{
		queue.addTaskCount();
		queue.push(taskPtr);
	}

	void TaskScheduler::runNext(Queue& queue)
	{
		TaskPtr ptr = nullptr;

		if (queue.try_pop(ptr))
		{
			auto handle = std::coroutine_handle<TaskPromiseBase>::from_address(ptr);
			auto& promise = handle.promise();
			if (promise.resume())
			{
				if (promise.tasksAwaiter_) [[likely]]
				{
					const std::size_t count = promise.tasksAwaiter_->tasks_.size() - 1;
					if (count > 0)
						queue.addTaskCount(count);
					for (TaskPtr ptr : promise.tasksAwaiter_->tasks_)
					{
						auto h = std::coroutine_handle<TaskPromiseBase>::from_address(ptr);
						h.promise().parentCoroutine_ = handle;
						queue.push(ptr);
					}
				}
				else [[unlikely]]
				{
					puts("enqueue resuming task... [SHOULD NEVER HAPPEN!!]");
				}
			}
			else if (promise.parentCoroutine_)
			{
				auto& parentPromise = promise.parentCoroutine_.promise();
				if (parentPromise.tasksAwaiter_)
				{
					if (parentPromise.tasksAwaiter_->doneCount_.fetch_sub(1, std::memory_order::acq_rel) - 1 == 0)
					{
						queue.push(promise.parentCoroutine_.address());
					}
					else
					{
						queue.removeTaskCount(1);
					}
				}
				else
				{
					queue.push(promise.parentCoroutine_.address());
				}
				handle.destroy();
			}
			else
			{
				queue.removeTaskCount(1);
				handle.destroy();
			}
		}
	}

	void TaskScheduler::execute(Queue& queue)
	{
		while (!queue.isDone())
		{
			runNext(queue);
		}
	}

	void TaskScheduler::execute()
	{
		execute(queue_.value());
	}
}