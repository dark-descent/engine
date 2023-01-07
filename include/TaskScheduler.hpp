#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

#include "LockFreeQueue.hpp"
#include "PersistentVector.hpp"
#include "Logger.hpp"
#include "Task.hpp"

namespace DarkDescent
{
	class TaskScheduler: public SubSystem
	{
	public:
		struct Queue: public LockFree::Queue<TaskPtr>
		{
			Queue(std::size_t count): LockFree::Queue<TaskPtr>(count), tasksCount_(0) { }

			inline bool isDone() const noexcept
			{
				const std::size_t count = tasksCount_.load(std::memory_order::acquire);
				return count == 0;
			}

			inline void addTaskCount(std::size_t count = 1) noexcept { tasksCount_.fetch_add(count, std::memory_order::acq_rel); };
			inline void removeTaskCount(std::size_t count = 1) noexcept { tasksCount_.fetch_sub(count, std::memory_order::acq_rel); };

		private:
			std::atomic<std::size_t> tasksCount_;
		};

		SUB_SYSTEM_CTORS(TaskScheduler)
		{ }

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	private:
		void runNext(Queue& queue);

		void schedule(Queue& queue, TaskPtr taskPtr);

		template<typename T>
		void buildTasksList(std::vector<TaskPtr>& ptrs, T task)
		{
			ptrs.emplace_back(task.handle().address());
		}

		template<typename T, typename... Tasks>
		void buildTasksList(std::vector<TaskPtr>& ptrs, T task, Tasks... tasks)
		{
			buildTasksList(ptrs, std::forward<T>(task));
			buildTasksList(ptrs, std::forward<Tasks>(tasks)...);
		}

	public:
		template<typename T>
		void schedule(Task<T> task)
		{
			schedule(queue_.value(), task.handle().address());
		}

		template<typename Task, typename... Tasks>
		void schedule(Task task, Tasks&&... tasks)
		{
			schedule(queue_.value(), task);
			schedule(queue_.value(), std::forward<Tasks>(tasks)...);
		}

		template<typename T>
		void schedule(Queue& queue, Task<T> task)
		{
			schedule(queue, task.handle().address());
		}

		template<typename Task, typename... Tasks>
		void schedule(Queue& queue, Task task, Tasks&&... tasks)
		{
			schedule(queue, task);
			schedule(queue, std::forward<Tasks>(tasks)...);
		}

		void execute(Queue& queue);
		void execute();

		template<typename T>
		TasksAwaiter awaitTasks(T task)
		{
			std::vector<TaskPtr> taskPtrs;
			buildTasksList(taskPtrs, std::forward<T>(task));
			return TasksAwaiter(taskPtrs);
		}

		template<typename... T>
		TasksAwaiter awaitTasks(T... tasks)
		{
			std::vector<TaskPtr> taskPtrs;
			buildTasksList(taskPtrs, std::forward<T>(tasks)...);
			return TasksAwaiter(taskPtrs);
		}

		template<typename T>
		TasksAwaiter awaitTasks(std::vector<Task<T>>& tasks)
		{
			const size_t l = tasks.size();
			std::vector<TaskPtr> taskPtrs(l);
			for (std::size_t i = 0; i < l; i++)
				taskPtrs[i] = tasks.at(i).handle().address();
			return TasksAwaiter(taskPtrs);
		}

		template<>
		TasksAwaiter awaitTasks(std::vector<Task<>>& tasks)
		{
			const size_t l = tasks.size();
			std::vector<TaskPtr> taskPtrs(l);
			for (std::size_t i = 0; i < l; i++)
				taskPtrs[i] = tasks.at(i).handle().address();
			return TasksAwaiter(taskPtrs);
		}

		template<typename Container, typename Callback>
		TasksAwaiter awaitEach(Container& container, Callback callback)
		{
			std::vector<TaskPtr> taskPtrs;
			for (auto& item : container)
				buildTasksList(taskPtrs, callback(item));
			return TasksAwaiter(taskPtrs);
		}

		template<typename Callback, typename IndexType = std::size_t>
		TasksAwaiter awaitEach(IndexType count, Callback callback)
		{
			std::vector<TaskPtr> taskPtrs;
			for (IndexType i = 0; i < count; i++)
				buildTasksList(taskPtrs, callback(i));
			return TasksAwaiter(taskPtrs);
		}

	private:
		std::optional<Queue> queue_;
		std::vector<std::thread> workerThreads_;
		std::atomic<bool> isRunning_;
	};
}