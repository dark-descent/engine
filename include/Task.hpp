#pragma once

#include "pch.hpp"

#define TASK(NAME) Task NAME(TaskScheduler& scheduler, void* data)

namespace DarkDescent
{
	class TaskScheduler;

	struct Task
	{
		using Counter = std::atomic<std::uint32_t>;
	
		struct promise_type
		{
			Task get_return_object()
			{
				return { std::coroutine_handle<promise_type>::from_promise(*this) };
			}

			std::suspend_always initial_suspend() const noexcept { return {}; }
			std::suspend_never final_suspend() const noexcept { return {}; }

			bool await_ready() const noexcept { return false; }
			void await_suspend(std::coroutine_handle<promise_type>) const noexcept { }
			void await_resume() const noexcept { }

			void return_void() noexcept
			{
				isDone_ = true;
				if (dependencyCounter_ != nullptr)
					dependencyCounter_->fetch_sub(1, std::memory_order::acq_rel);
				dependencyCounter_ = nullptr;
			}

			void unhandled_exception() const { std::abort(); }

			bool isDone() const noexcept { return isDone_; }
			
			bool isWaiting() const noexcept
			{
				if(isDone_ || (counter_ == nullptr))
					return false;
				return counter_->load(std::memory_order::acquire) != 0;
			}

			void setCounter(Task::Counter* counter)
			{
				assert(counter != nullptr);
				assert(counter_ == nullptr);
				counter_ = counter;
			}

			void setDependencyCounter(Task::Counter* counter)
			{
				assert(counter != nullptr);
				assert(dependencyCounter_ == nullptr);
				dependencyCounter_ = counter;
			}

			promise_type():
				isDone_(false),
				dependencyCounter_(nullptr),
				counter_(nullptr)
			{}

		private:
			bool isDone_;
			std::atomic<std::uint32_t>* dependencyCounter_;
			std::atomic<std::uint32_t>* counter_ = 0;
		};
		
		using Handle = std::coroutine_handle<promise_type>;

		struct Info
		{
			using Function = Task(*)(TaskScheduler&, void*);
			Function function;
			void* arg = nullptr;
		};

		Task(Handle handle): handle_(handle) { }

		Handle handle_;
		operator Handle() const { return handle_; }
	};
};