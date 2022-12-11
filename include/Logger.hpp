#pragma once

#include "pch.hpp"
#include "Concepts.hpp"

namespace DarkDescent
{
	class Logger
	{
#ifdef _WIN32
		using Color = WORD;
#else
		using Color = const char*;
#endif

	public:
		enum class LogSeverity
		{
			INFO,
			WARNING,
			ERROR,
			DEBUG,
			EXCEPTION,
		};

	private:
		constexpr static unsigned long long severities = static_cast<unsigned long long>(LogSeverity::EXCEPTION) + 1;

		struct LogInfo
		{
			const Logger* logger;
			std::thread::id threadID;
			LogSeverity sevirity;
			std::string data;

			LogInfo(const Logger* logger, LogSeverity sevirity, std::string&& data):
				logger(logger),
				threadID(std::this_thread::get_id()),
				sevirity(sevirity),
				data(data)
			{ }
		};

		static const std::string& getCurrentDate();

#ifdef _WIN32
		static inline constexpr Color createColor(int foreground, int background = 0)
		{
			return ((background & 0x0F) << 4) + (foreground & 0x0F);
		}
#endif

	public:

		/**
		 * @brief Initializes the Logger system with an optional path to the log folder.
		 *
		 * @param path The path to the log folder where all the log files will be stored.
		 * @return true when the Logger succesfully initializes
		 * @return false when the Logger already is initialized
		 */

		static bool initialize(std::filesystem::path&& path);
		static bool initialize(const char* path = nullptr);
		static const Logger& get(const char* name = nullptr);
		static void terminate();

	private:
		Logger(const char* path);
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		~Logger();

		void print(LogInfo& info) const;

		inline std::string convertToString(const char* str) const { return std::string(str); }
		inline std::string convertToString(const std::string& str) const noexcept { return str; }
		inline std::string convertToString(std::string&& str) const noexcept { return str; }
		inline std::string convertToString(bool boolValue) const noexcept { return boolValue ? "true" : "false"; }

		template <typename T>
			requires IsLoggable<T>
		std::string convertToString(T& loggable) const
		{
			return std::string(loggable.log());
		}

		template <typename T, typename... Rest>
			requires IsLoggable<T>
		std::string convertToString(T& loggable, Rest... rest) const
		{
			return std::string(loggable.log()) + convertToString(rest...);
		}

		template <typename... Rest>
		std::string convertToString(const char* str, Rest... rest) const { return std::string(str) + convertToString(rest...); }

		template <typename... Rest>
		std::string convertToString(const std::string& str, Rest... rest) const { return str + convertToString(rest...); }

		template <typename... Rest>
		std::string convertToString(std::string&& str, Rest... rest) const { return str + convertToString(rest...); }

		template <typename T>
		std::string convertToString(T arg) const
		{
			return std::to_string(arg);
		}
		

		template <typename T, typename... Rest>
		std::string convertToString(T arg, Rest... rest) const
		{
			return convertToString(arg) + convertToString(rest...);
		}

		std::string getPrefix(LogSeverity severity) const;

		void forward(LogSeverity sevirity, std::string&& data) const;

	public:
		void log(LogSeverity severity, const char* arg) const
		{
			std::string data = getPrefix(severity) + arg;
			forward(severity, std::move(data));
		}

		template <typename T>
		void log(LogSeverity severity, T arg) const
		{
			std::string data = getPrefix(severity) + convertToString(arg);
			forward(severity, std::move(data));
		}

		template <typename T, typename... Rest>
		void log(LogSeverity severity, T arg, Rest... rest) const
		{
			std::string data = getPrefix(severity) + convertToString(arg, rest...);
			forward(severity, std::move(data));
		}

		template <typename T>
		void debug(T arg) const
		{
			log(LogSeverity::DEBUG, arg);
		}

		template <typename T, typename... Rest>
		void debug(T arg, Rest... rest) const
		{
			log(LogSeverity::DEBUG, arg, rest...);
		}


		template <typename T>
		void info(T arg) const
		{
			log(LogSeverity::INFO, arg);
		}

		template <typename T, typename... Rest>
		void info(T arg, Rest... rest) const
		{
			log(LogSeverity::INFO, arg, rest...);
		}


		template <typename T>
		void warn(T arg) const
		{
			log(LogSeverity::WARNING, arg);
		}

		template <typename T, typename... Rest>
		void warn(T arg, Rest... rest) const
		{
			log(LogSeverity::WARNING, arg, rest...);
		}


		template <typename T>
		void error(T arg) const
		{
			log(LogSeverity::ERROR, arg);
		}

		template <typename T, typename... Rest>
		void error(T arg, Rest... rest) const
		{
			log(LogSeverity::ERROR, arg, rest...);
		}



		template <typename T>
		void exception(T arg) const
		{
			log(LogSeverity::EXCEPTION, arg);
		}

		template <typename T, typename... Rest>
		void exception(T arg, Rest... rest) const
		{
			log(LogSeverity::EXCEPTION, arg, rest...);
		}

	private:
		static std::filesystem::path logDir_;
		static std::optional<std::thread> logHandlerThread_;
		static std::mutex mutex_;
		static std::condition_variable cv_;

		static std::unordered_map<std::string, Logger*> loggers_;
		static std::queue<LogInfo> logQueue_;

		static bool shouldTerminate_;

		static const char* prefixes[severities];
		static Color severityColors[severities];
		static Color resetColor;

		std::string path_;
		mutable std::ofstream logFile_;

#ifdef _WIN32
		HANDLE hConsole;
#endif
	};
}