#include "Bin.hpp"

namespace DarkDescent::Bin
{
	TypeInfo::TypeInfo(): TypeInfo(0, 0, nullptr) { }
	TypeInfo::TypeInfo(std::size_t type, std::size_t size): TypeInfo(type, size, nullptr) { }
	TypeInfo::TypeInfo(std::size_t type, std::size_t size, const char* ptr): type(type), size(size), ptr(ptr) { }

	Writer::Writer(std::string path): file_(path) { }

	void Writer::flushHeader(std::ofstream& os)
	{
		std::size_t size = header_.size() * sizeof(TypeInfo);
		os.write(reinterpret_cast<const char*>(&size), sizeof(std::size_t));
		os.write(reinterpret_cast<const char*>(header_.data()), size);
	}

	void Writer::flush()
	{
		std::ofstream os = std::ofstream(file_, std::ofstream::binary);

		flushHeader(os);

		std::size_t infoIndex = 0;

		const std::size_t il = header_.size();

		TypeInfo info = header_[infoIndex];

		while (infoIndex < il)
		{
			info = header_[infoIndex];
			if ((info.type & Type::END) == Type::END)
			{
				infoIndex++;
			}
			else
			{
				switch (info.type)
				{
					case Type::VECTOR:
						infoIndex = flushVector(os, infoIndex + 1, info.size);
						break;
					default:
						os.write(info.ptr, info.size);
						infoIndex++;
						break;
				}
			}
		}

		os.flush();
	}

	std::size_t Writer::flushVector(std::ofstream& os, std::size_t nextIndex, std::size_t size)
	{
		TypeInfo& next = header_.at(nextIndex);

		if (next.type == Type::VECTOR)
			return flushVector(os, nextIndex + 1, next.size);
		else if (next.type == Type::STRING)
		{
			for (std::size_t i = 0; i < size; i++)
			{
				next = header_.at(nextIndex + i);
				os.write(next.ptr, next.size);
			}
			return nextIndex + size;
		}

		os.write(next.ptr, size * next.size);

		return nextIndex + 1;
	}

	Reader::Reader(std::string file): file_(file) { }

	Parser::Parser(std::ifstream& is) : is_(is), header_(), infoIndex_(0)
	{
		std::size_t size = 0;
		is.read(reinterpret_cast<char*>(&size), sizeof(std::size_t));
		header_.resize(size);
		is.read(reinterpret_cast<char*>(header_.data()), size);
	}

	void Parser::read(char* const ptr)
	{
		if (infoIndex_ >= header_.size())
			return;

		TypeInfo& info = header_.at(infoIndex_);

		if ((info.type & Type::END) == Type::END)
		{
			infoIndex_++;
			return read(ptr);
		}
		else if (info.type == Type::VECTOR)
		{
			infoIndex_ = readVector(ptr, infoIndex_ + 1, info.size);
		}
		else if (info.type == Type::STRING)
		{
			std::string* str = reinterpret_cast<std::string*>(ptr);
			str->resize(info.size, '\0');
			is_.read(str->data(), info.size);
		}
		else
		{
			is_.read(ptr, info.size);
			infoIndex_++;
		}
	}

	std::size_t Parser::readVector(char* const ptr, std::size_t nextIndex, std::size_t size)
	{
		TypeInfo& next = header_.at(nextIndex);

		while ((next.type & Type::END) == Type::END)
		{
			nextIndex++;
			if (nextIndex >= header_.size())
				return nextIndex;
			next = header_.at(nextIndex);
		}

		if (next.type == Type::VECTOR)
		{
			std::vector<std::vector<char>>* vec = reinterpret_cast<std::vector<std::vector<char>>*>(ptr);
			vec->resize(size);
			for (std::size_t i = 0; i < size; i++)
			{
				vec->at(i) = std::vector<char>();
				nextIndex = readVector(reinterpret_cast<char*>(&vec->at(i)), nextIndex + 1, header_.at(nextIndex).size);
			}
			return nextIndex + 1;
		}
		else if (next.type == Type::STRING) // vector of strings
		{
			std::vector<std::string>* vec = reinterpret_cast<std::vector<std::string>*>(ptr);

			for (std::size_t i = 0; i < size; i++)
			{
				next = header_.at(nextIndex + i);
				std::string& str = vec->emplace_back(next.size, '\0');
				is_.read(str.data(), next.size);
			}

			return nextIndex + size;
		}
		else
		{
			const std::size_t blockSize = size * next.size;
			std::vector<char>* vec = reinterpret_cast<std::vector<char>*>(ptr);
			vec->resize(blockSize);
			is_.read(vec->data(), blockSize);
			return nextIndex + 2;
		}

	}
}