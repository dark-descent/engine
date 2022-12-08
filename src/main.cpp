#include "pch.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"

struct Test
{
	int i;

	Test(int i): i(i) { }

	std::string log() const
	{
		return std::format("struct Test::i = {}", i);
	}
};

void testStackTrace()
{
	int i = 0;
	int j = 1000;
	throw DarkDescent::TraceException("hihi");
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	using namespace DarkDescent;

	Logger::initialize(std::filesystem::current_path() / ".." / ".." / ".." / "logs");
	
	const Logger& logger = Logger::get();

	try
	{
		Test testObj(1);

		logger.log(Logger::LogSeverity::DEBUG, 1, " - ", true, " - ", testObj);

		testStackTrace();
	}
	catch (const TraceException& e)
	{
		logger.log(Logger::LogSeverity::EXCEPTION, e);
	}

	Logger::terminate();

	return 0;
}