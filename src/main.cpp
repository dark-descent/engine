#include "pch.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"
#include "Engine.hpp"
#include "PersistentAllocator.hpp"

#if defined(_DEBUG) && defined(_WIN32)
void setupWinConsole(bool bindStdIn = true, bool bindStdOut = true, bool bindStdErr = true);
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	using namespace DarkDescent;

#if defined(_DEBUG) && defined(_WIN32)
	setupWinConsole();
#endif

	if (!Logger::initialize(std::filesystem::current_path() / ".." / ".." / ".." / "logs"))
		return 1;

	const Logger& logger = Logger::get();

	try
	{
		auto& engine = Engine::initialize("../../../game.json");

		engine.run();

		Engine::terminate();
	}
	catch (const TraceException& e)
	{
		logger.exception(e);
		return 2;
	}

#if defined(_DEBUG) && defined(_WIN32)
	logger.info("Press a key to exit...");
	getchar();
	FreeConsole();
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