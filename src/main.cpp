#include "pch.hpp"
#include "Logger.hpp"

struct Test
{
	int i;

	Test(int i): i(i) { }

	std::string log() const
	{
		return std::format("struct Test::i = {}", i);
	}
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	using namespace DarkDescent;

	Logger::initialize(std::filesystem::current_path() / ".." / ".." / ".." / "logs");

	const Logger& logger = Logger::get();

	Test testObj(1);

	logger.log(Logger::LogSeverity::DEBUG, 1, " - ", true, " - ", testObj);

	Logger::terminate();

	return 0;
}