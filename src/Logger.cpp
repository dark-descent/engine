#include "pch.hpp"

#include "Logger.hpp"

namespace DarkDescent
{
	const char* Logger::prefixes[Logger::severities] = {
		"[INFO] ",
		"[WARNING] ",
		"[ERROR] ",
		"[DEBUG] ",
		"[EXCEPTION] " };

#ifdef _WIN32
	Logger::Color Logger::resetColor = Logger::createColor(15, 0);

	Logger::Color Logger::severityColors[Logger::severities] = {
		Logger::createColor(9),
		Logger::createColor(14),
		Logger::createColor(4),
		Logger::createColor(1),
		Logger::createColor(4),
	};
#else
	Logger::Color Logger::resetColor = "\033[39m\033[49m";

	Logger::Color Logger::levelColors[Logger::severities] = {
		"\033[34m",
		"\033[33m",
		"\033[31m",
		"\033[30m",
		"\033[31m",
	};
#endif

	std::filesystem::path Logger::logDir_;
	std::unordered_map<std::string, Logger*> Logger::loggers_ = std::unordered_map<std::string, Logger*>();
	std::queue<Logger::LogInfo> Logger::logQueue_;
	std::mutex Logger::mutex_;
	std::condition_variable Logger::cv_;
	bool Logger::shouldTerminate_ = false;
	std::optional<std::thread> Logger::logHandlerThread_;

	const std::string& Logger::getCurrentDate()
	{
		static std::optional<std::string> dateString;
		if (!dateString.has_value())
		{
			std::stringstream ss;
			time_t t = time(0);
			tm now;
			localtime_s(&now, &t);
			ss << (now.tm_year + 1900) << '-' << (now.tm_mon + 1) << '-' << now.tm_mday;
			dateString = ss.str();
		}
		return dateString.value();
	}

	bool Logger::initialize(std::filesystem::path&& path)
	{
		return initialize(path.string().c_str());
	}

	/**
	 * TODO:
	 * 	- let the log method calculate its time instead of when it arrives at the background thread
	 */
	bool Logger::initialize(const char* path)
	{
		if (!logHandlerThread_.has_value())
		{
			if (path != nullptr)
				logDir_ = path;
			else
				logDir_ = std::filesystem::current_path() / "logs";

			if (!std::filesystem::exists(logDir_))
				std::filesystem::create_directory(logDir_);

			logHandlerThread_.emplace(std::thread([ & ]()
			{
				time_t rawtime;
			tm timeinfo;
			char timeBuf[10] = {};

			while (!shouldTerminate_)
			{
				std::unique_lock<std::mutex> lock(mutex_);
				cv_.wait(lock);
				lock.unlock();

				while (logQueue_.size() > 0)
				{
					Logger::LogInfo& info = logQueue_.front();

					time(&rawtime);
					localtime_s(&timeinfo, &rawtime);
					strftime(timeBuf, 10, "%T", &timeinfo);

					info.logger->logFile_ << "[" << timeBuf << "] ";

					info.logger->logFile_ << info.data << '\n';
					info.logger->logFile_.flush();

					lock.lock();
					logQueue_.pop();
					lock.unlock();
				}
			}

			}));

			return true;
		}
		return false;
	}

	const Logger& Logger::get(const char* name)
	{
		if (!logHandlerThread_.has_value())
			throw std::runtime_error("Cannot get logger before a call to Logger::initialize(...)!");

		std::string fileName;

		if (name == nullptr)
			fileName = Logger::getCurrentDate();
		else
			fileName = std::string(name) + "-" + Logger::getCurrentDate();

		if (!loggers_.contains(fileName))
		{
			size_t version = 0;

			bool exists;

			std::filesystem::path logFilePath;

			do
			{
				std::string versionString = version == 0 ? "" : (std::string("-") + std::to_string(version));
				std::string genName = fileName + versionString + ".log";

				logFilePath = logDir_ / genName;

				exists = std::filesystem::exists(logFilePath);

				if (exists)
					version++;

			} while (exists);

			loggers_[fileName] = new Logger(logFilePath.string().c_str());
		}

		return *loggers_[fileName];
	}

	void Logger::terminate()
	{
		shouldTerminate_ = true;

		if (logHandlerThread_.has_value())
		{
			cv_.notify_one();
			if (logHandlerThread_.value().joinable())
				logHandlerThread_.value().join();
			logHandlerThread_.reset();
		}

		loggers_.clear();

		shouldTerminate_ = false;
	}

	Logger::Logger(const char* path):
		logFile_(path)
	{
#ifdef _WIN32
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	Logger::~Logger()
	{
		if (logFile_.is_open())
			logFile_.close();
	}

	void Logger::print(LogInfo& info) const
	{

	}

	std::string Logger::getPrefix(LogSeverity level) const
	{
		const unsigned long long i = static_cast<unsigned long long>(level);
		return prefixes[i];
	}

	void Logger::forward(Logger::LogSeverity sevirity, std::string&& data) const
	{
		std::unique_lock<std::mutex> lock(mutex_);
		logQueue_.emplace(std::move(LogInfo(this, sevirity, std::forward<std::string>(data))));
		cv_.notify_one();
	}
}