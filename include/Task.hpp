#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	using TaskPtr = void*;

	template<typename T = void>
	class Task;

	template<typename T = void>
	struct TaskPromise;

	struct TasksAwaiter;

	struct TaskPromiseBase
	{
		struct Allocator
		{
			static void* alloc(std::size_t size)
			{
				// printf("----- alloc coroutine size: %zu\n", size); 
				return ::malloc(size);
			}
			static void free(void* ptr, std::size_t size)
			{
				// printf("-----  free coroutine size: %zu\n", size); 
				::free(ptr);
			};
		};

		std::suspend_always initial_suspend() const noexcept
		{
			return {};
		}

		std::suspend_always final_suspend() const noexcept
		{
			return {};
		}

		void unhandled_exception()
		{
			exception_.emplace(std::current_exception());
		}

		virtual bool resume() = 0;

		virtual bool done() const noexcept { return exception_.has_value() || false; }

		std::optional<std::exception_ptr> exception_ = {};
		std::coroutine_handle<TaskPromiseBase> parentCoroutine_ = nullptr;
		TasksAwaiter* tasksAwaiter_ = nullptr;

		TaskPromiseBase()
		{
			// puts("TaskPromise()");
		}


		~TaskPromiseBase()
		{
			// puts("~TaskPromise()");
		}

		void* operator new(std::size_t size)
		{
			return Allocator::alloc(size);
		}

		void operator delete(void* ptr, std::size_t size)
		{
			return Allocator::free(ptr, size);
		}
	};

	template<>
	struct TaskPromise<void>: public TaskPromiseBase
	{
		void return_void() noexcept
		{
			isDone_ = true;
		}

		virtual bool resume()
		{
			auto handle = std::coroutine_handle<TaskPromise<void>>::from_promise(*this);
			handle.resume();
			return !done();
		}

		virtual bool done() const noexcept override { return exception_.has_value() || isDone_; }

		std::size_t frameSize_;
		bool isDone_ = false;
	};

	template<typename T>
	struct TaskPromise: public TaskPromiseBase
	{
		Task<T> get_return_object() noexcept
		{
			return Task<T>(std::coroutine_handle<TaskPromise<T>>::from_promise(*this));
		}

		void return_value(const T& v) noexcept
		{
			try
			{
				value_.emplace(v);
			}
			catch (...)
			{
				exception_.emplace(std::current_exception());
			}
		}

		void return_value(T&& v) noexcept
		{
			try
			{
				value_.emplace(std::forward<T>(v));
			}
			catch (...)
			{
				exception_.emplace(std::current_exception());
			}
		}

		virtual bool done() const noexcept override { return exception_.has_value() || value_.has_value(); }

		virtual bool resume()
		{
			auto handle = std::coroutine_handle<TaskPromiseBase>::from_promise(*this);
			handle.resume();
			return !done();
		}

		std::optional<T> value_;
	};

	struct TasksAwaiter
	{
		void await_resume() const noexcept
		{
			// puts("await_resume void");
		}

		template<typename T>
		void await_suspend(std::coroutine_handle<T> caller)
		{
			// puts("await_suspend");
			caller.promise().tasksAwaiter_ = this;
		}

		bool await_ready()
		{
			// puts("await_ready");
			return doneCount_.load(std::memory_order::acquire) == 0;
		}

		TasksAwaiter(std::vector<TaskPtr> tasks):
			tasks_(std::move(tasks)),
			doneCount_(tasks_.size())
		{ }

		~TasksAwaiter()
		{
			// puts("~TasksAwaiter()");
			for (auto t : tasks_)
			{
				auto h = std::coroutine_handle<TaskPromiseBase>::from_address(t);
				h.destroy();
			}

		}

		std::vector<TaskPtr> tasks_;
		std::atomic<std::size_t> doneCount_;
	};

	template<typename T>
	struct SingleTaskAwaiter: public TasksAwaiter
	{
		T await_resume() const noexcept
		{
			// puts("await_resume T");
			std::coroutine_handle<TaskPromise<T>> a = std::coroutine_handle<TaskPromise<T>>::from_address(tasks_.at(0));
			return a.promise().value_.value();
		}

		SingleTaskAwaiter(std::vector<TaskPtr> tasks): TasksAwaiter(tasks) { }
	};

	template<>
	class Task<void>
	{
	public:
		struct PromiseType: public TaskPromise<void>
		{
			Task<void> get_return_object() noexcept
			{
				return Task<void>(std::coroutine_handle<promise_type>::from_promise(*this));
			}
		};

		using promise_type = PromiseType;

		Task() noexcept: handle_(nullptr) { };
		Task(std::coroutine_handle<promise_type> handle) noexcept: handle_(handle) { }

		TasksAwaiter operator co_await()
		{
			return TasksAwaiter({ handle_.address() });
		}

		std::coroutine_handle<promise_type> handle() const noexcept { return handle_; };

	private:
		std::coroutine_handle<promise_type> handle_;
	};

	template<typename T>
	class Task
	{
	public:
		using promise_type = TaskPromise<T>;

		Task() noexcept: handle_(nullptr) { };
		Task(std::coroutine_handle<promise_type> handle) noexcept: handle_(handle) { }

		SingleTaskAwaiter<T> operator co_await()
		{
			return SingleTaskAwaiter<T>({ handle_.address() });
		}

		std::coroutine_handle<promise_type> handle() const noexcept { return handle_; };

	private:
		std::coroutine_handle<promise_type> handle_;
	};
}