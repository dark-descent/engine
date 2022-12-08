#include "pch.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"
#include "Bin.hpp"

#if defined(_DEBUG) && defined(_WIN32)
void setupWinConsole(bool bindStdIn = true, bool bindStdOut = true, bool bindStdErr = true);
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	using namespace DarkDescent;

#if defined(_DEBUG) && defined(_WIN32)
	setupWinConsole();
#endif

	Logger::initialize(std::filesystem::current_path() / ".." / ".." / ".." / "logs");

	const Logger& logger = Logger::get();

	try
	{
		std::filesystem::path p = std::filesystem::current_path() / ".." / ".." / ".." / "test.bin";

		struct Test
		{
			double a;
			int b;

			std::string log() { return std::format("a: {}, b: {}", a, b); }
		};

		{
			Bin::Writer writer(p.string());

			std::vector<std::string> strings = {
				std::string("Helloa A :D"),
				std::string("Helloa B :D"),
				std::string("Helloa C :D")
			};

			double second = 123.5;

			Test testStruct = {
				.a = 12345.123,
				.b = 112233
			};

			writer.write(strings).write(second).write(testStruct);

			auto start = std::chrono::high_resolution_clock::now();

			writer.flush();

			auto finish = std::chrono::high_resolution_clock::now();
			auto delta = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
			logger.info("Flushed in ", delta.count(), " microseconds");
		}

		std::vector<std::chrono::microseconds> durations;

		for (size_t i = 0; i < 10; i++)
		{
			Bin::Reader reader(p.string());

			std::vector<std::string> test;
			double second = 0.0;
			Test testStruct;

			auto start = std::chrono::high_resolution_clock::now();

			reader.read([ & ](Bin::Parser& parser)
			{
				parser.read(test);
			parser.read(second);
			parser.read(testStruct);
			});

			auto finish = std::chrono::high_resolution_clock::now();
			auto delta = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
			durations.push_back(delta);

			logger.info("Parsed in ", delta.count(), " microseconds");


			std::vector<size_t*> cacheTrasher;

			for (size_t i = 0; i < 1024 * 64; i++)
			{
				cacheTrasher.emplace_back(new size_t(i));
			}


			for (size_t i = 0; i < 1024 * 64; i++)
			{
				delete cacheTrasher[i];
			}

			for (const auto& str : test)
				logger.info(str.c_str());

			logger.info("second: ", second);
			logger.info("struct: ", testStruct);

		}

		long long total = 0;

		for (size_t i = 0; i < 10; i++)
		{
			total += durations[i].count();
		}

		double average = static_cast<double>(total) / 10.0;

		logger.info("Parsed 10 times in ", total, " microseconds");
		logger.info("Average parse time ", average, " microseconds");
	}
	catch (const TraceException& e)
	{
		logger.log(Logger::LogSeverity::EXCEPTION, e);
	}

#if defined(_DEBUG) && defined(_WIN32)
	getchar();
#endif

	Logger::terminate();

	return 0;
}

#if defined(_DEBUG) && defined(_WIN32)
void setupWinConsole(bool bindStdIn, bool bindStdOut, bool bindStdErr)
{
	AllocConsole();

	if (bindStdIn)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "CONIN$", "r", stdin);
	}
	if (bindStdOut)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "CONOUT$", "w", stdout);
	}
	if (bindStdErr)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "CONOUT$", "w", stderr);
	}

	// Redirect unbuffered stdin from the current standard input handle
	if (bindStdIn)
	{
		HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
		if (stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if (fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "r");
				if (file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stdin));
					if (dup2Result == 0)
					{
						setvbuf(stdin, NULL, _IONBF, 0);
					}
				}
			}
		}
	}

	// Redirect unbuffered stdout to the current standard output handle
	if (bindStdOut)
	{
		HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if (fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "w");
				if (file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stdout));
					if (dup2Result == 0)
					{
						setvbuf(stdout, NULL, _IONBF, 0);
					}
				}
			}
		}
	}

	// Redirect unbuffered stderr to the current standard error handle
	if (bindStdErr)
	{
		HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
		if (stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if (fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "w");
				if (file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stderr));
					if (dup2Result == 0)
					{
						setvbuf(stderr, NULL, _IONBF, 0);
					}
				}
			}
		}
	}

	// Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the
	// standard streams before they refer to a valid target will cause the iostream objects to enter an error state. In
	// versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything
	// has been read from or written to the targets or not.
	if (bindStdIn)
	{
		std::wcin.clear();
		std::cin.clear();
	}
	if (bindStdOut)
	{
		std::wcout.clear();
		std::cout.clear();
	}
	if (bindStdErr)
	{
		std::wcerr.clear();
		std::cerr.clear();
	}
}
#endif