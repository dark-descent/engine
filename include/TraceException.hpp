#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class TraceException : public std::exception
	{
		using StackTrace = std::basic_stacktrace<std::allocator<std::stacktrace_entry>>;

		public:
			TraceException(const char* what, StackTrace stackTrace = std::stacktrace::current()) noexcept : 
				std::exception(what),
				stackTrace_(stackTrace)
			{  }

			std::string stackTrace() const { return std::to_string(stackTrace_); }

			std::string log() const { return std::string(what()) + "\n" + stackTrace(); }

		private:
			const StackTrace stackTrace_;
	};
}