#include "pch.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"
#include "Bin.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	using namespace DarkDescent;

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

		// {
		// 	Bin::Writer writer(p.string());

		// 	std::vector<std::string> strings = {
		// 		std::string("Helloa A :D"),
		// 		std::string("Helloa B :D"),
		// 		std::string("Helloa C :D")
		// 	};

		// 	double second = 123.5;

		// 	Test testStruct = {
		// 		.a = 12345.123,
		// 		.b = 112233
		// 	};

		// 	writer.write(strings).write(second).write(testStruct);

		// 	auto start = std::chrono::high_resolution_clock::now();

		// 	writer.flush();

		// 	auto finish = std::chrono::high_resolution_clock::now();
		// 	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
		// 	logger.info("Flushed in ", delta.count(), " microseconds");
		// }

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
			logger.info("Parsed in ", delta.count(), " microseconds");

			// for (const auto& str : test)
			// 	logger.info(str.c_str());

			// logger.info("second: ", second);
			// logger.info("struct: ", testStruct);
		}
	}
	catch (const TraceException& e)
	{
		logger.log(Logger::LogSeverity::EXCEPTION, e);
	}

	Logger::terminate();

	return 0;
}