#include "TaskScheduler.hpp"
#include "Logger.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	void TaskScheduler::onInitialize()
	{
		// TaskSystemConfig& config = engine()->configManager.getTaskSystemConfig();
		// maxTasks_ = config.maxTasks == 0 ? DefaultConfig::TaskSystem::maxTasks : config.maxTasks;
		executionThreads_ = 2; //(config.executionThreads == 0 ? std::thread::hardware_concurrency() : config.executionThreads) - 1;
		threadIdMap_.emplace(std::this_thread::get_id(), 0);
		readyQueue_ = new LockFree::Queue<TaskHandle>(maxTasks_);
		mainThreadQueue_ = new LockFree::Queue<TaskHandle>(maxTasks_);
	}

	void TaskScheduler::onReady()
	{
		
	}

	void TaskScheduler::onTerminate()
	{
		if (threadsRunning_.load(std::memory_order::acquire) == 1)
			stopThreads();

		joinThreads();
	}

	void TaskScheduler::threadEntry(size_t threadID)
	{
		while (threadsRunning_.load(std::memory_order::acquire) != 1)
			; // wait (spin lock)

		logger.info("Thread with threadID ", std::to_string(threadID), " started...");

		TaskHandle taskHandle;
		while (threadsRunning_.load(std::memory_order::acquire) == 1)
		{
			if (runNextTask(taskHandle))
				handleTaskYield(taskHandle);
		}
	}

	void TaskScheduler::execNext(TaskHandle& TaskHandle)
	{
		if (runNextTask(TaskHandle))
			handleTaskYield(TaskHandle);
	}

	bool TaskScheduler::runNextTask(TaskHandle& handleOut)
	{
		if (readyQueue_->try_pop(handleOut))
		{
			if (handleOut != nullptr && !handleOut.done())
			{
				handleOut.resume();
				return true;
			}
			return false;
		}

		return false;
	}

	bool TaskScheduler::runNextMainThreadTask(TaskHandle& handleOut)
	{
		if (mainThreadQueue_->try_pop(handleOut))
		{
			if (!handleOut.done())
			{
				handleOut.resume();
				return true;
			}
		}
		return false;
	}

	Task::Counter* TaskScheduler::runTasks(TaskInfo* tasks, size_t tasksCout, bool mainThreadOnly)
	{
		Task::Counter* c = new Task::Counter(0);
		c->store(tasksCout, std::memory_order::relaxed);

		LockFree::Queue<TaskHandle>* targetQueue = mainThreadOnly ? mainThreadQueue_ : readyQueue_;

		for (size_t i = 0; i < tasksCout; i++)
			targetQueue->push(tasks[i].function(c, this, engine_, tasks[i].arg));

		return c;
	}

	Task::Counter* TaskScheduler::runTask(TaskInfo task, bool mainThreadOnly)
	{
		return runTasks(&task, 1, mainThreadOnly);
	}

	Task::Counter* TaskScheduler::runTask(TaskFunction function, bool mainThreadOnly)
	{
		return runTask({ function, 0 }, mainThreadOnly);
	}

	const bool TaskScheduler::isRunning() { return isRunning_; }

	void TaskScheduler::runThreads()
	{
		threadsRunning_.store(1, std::memory_order::release);
	}

	void TaskScheduler::stopThreads()
	{
		threadsRunning_.store(0, std::memory_order::release);
	}

	void TaskScheduler::joinThreads()
	{
		for (auto& thread : threads_)
			thread.join();
		threads_.clear();
	}

	size_t TaskScheduler::getThreadID()
	{
		return threadIdMap_[std::this_thread::get_id()];
	}

	void TaskScheduler::initThreads()
	{
		if (threads_.size() == 0)
			for (size_t i = 0; i < executionThreads_; i++)
			{
				threads_.push_back(std::thread([ & ] { threadEntry(i + 1); }));
				threadIdMap_.emplace(threads_[i].get_id(), i + 1);
			}
	}

	bool TaskScheduler::handleTaskYield(TaskHandle& handle)
	{
		if (handle == nullptr)
		{
			return false;
		}
		else
		{
			auto [counter, isDone] = handle.promise().state;

			if (!isDone)
			{
				size_t c = counter->load(std::memory_order::seq_cst);
				if (c == 0)
				{
					readyQueue_->push(handle);
				}
				else
				{
					while (!waitList_.pushWeak(handle))
					{
						c = counter->load(std::memory_order::seq_cst);
						if (c == 0)
						{
							readyQueue_->push(handle);
							return true;
						}
					}
				}
			}
			else
			{
				size_t index = counter->fetch_sub(1, std::memory_order::acq_rel) - 1;

				if (index == 0)
				{
					waitList_.findAndRelease([ & ](TaskHandle handle)
					{
						if (handle.promise().state.counter == counter)
						{
							readyQueue_->push(handle);
							return true;
						}
					return false;
					});
					handle.destroy();

					delete counter;
				}
			}
		}
		return true;
	}

	bool TaskScheduler::handleMainThreadTaskYield(TaskHandle& handle)
	{
		if (handle == nullptr)
		{
			return false;
		}
		else
		{
			auto [counter, isDone] = handle.promise().state;

			if (!isDone)
			{
				size_t c = counter->load(std::memory_order::acquire);
				if (c == 0)
				{
					mainThreadQueue_->push(handle);
				}
				else
				{
					while (!mainThreadWaitList_.pushWeak(handle))
					{
						c = counter->load(std::memory_order::acquire);
						if (c == 0)
						{
							mainThreadQueue_->push(handle);
							return true;
						}
					}
				}
			}
			else
			{
				size_t index = counter->fetch_sub(1, std::memory_order::acq_rel) - 1;

				if (index == 0)
				{
					mainThreadWaitList_.findAndRelease([ & ](TaskHandle handle)
					{
						if (handle.promise().state.counter == counter)
						{
							mainThreadQueue_->push(handle);
							return true;
						}
						return false;
					});
					handle.destroy();

					delete counter;
				}
			}
		}
		return true;
	}
}