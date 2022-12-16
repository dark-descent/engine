#pragma once

#include "pch.hpp"
#include "Concepts.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{

	namespace Bin
	{
		enum Type: uint64_t
		{
			U8,
			U16,
			U32,
			U64,
			I8,
			I16,
			I32,
			I64,
			F32,
			F64,
			BOOL,
			PRIMITIVES_SIZE,
			VECTOR,
			STRING,
			STRUCT,
			END = 0x10Ui64
		};

		constexpr uint8_t typeSizes[Bin::Type::PRIMITIVES_SIZE] = {
			1, 2, 4, 8,
			1, 2, 4, 8,
			4, 8, 1
		};

		struct TypeInfo
		{
			TypeInfo();
			TypeInfo(std::size_t type, std::size_t size);
			TypeInfo(std::size_t type, std::size_t size, const char* ptr);

			std::size_t type;
			std::size_t size;
			const char* ptr;
		};

		using Header = std::vector<TypeInfo>;

		class Writer
		{
		private:
			template<typename Test, template<typename...> class Ref>
			struct IsSpecialization: std::false_type { };

			template<template<typename...> class Ref, typename... Args>
			struct IsSpecialization<Ref<Args...>, Ref>: std::true_type { };

			template<typename T>
			void addPrimitiveType(Type type, T& ptr)
			{
				header_.emplace_back(type, typeSizes[type], reinterpret_cast<char*>(std::addressof(ptr)));
			}


		public:
			Writer(std::string file);
			Writer(const Writer&) = delete;
			Writer(Writer&&) = delete;

			template<typename T>
			Writer& write(std::vector<T>& vector)
			{
				const std::size_t vectorSize = vector.size();
				header_.push_back(TypeInfo(Type::VECTOR, vectorSize));
				if constexpr (IsSpecialization<T, std::vector>::value)
				{
					for (std::size_t i = 0; i < vectorSize; i++)
						write<T::value_type>(vector[i]);
				}
				else if constexpr (std::is_same<T, std::string>::value)
				{
					for (std::size_t i = 0; i < vectorSize; i++)
					{
						std::string& str = vector.at(i);
						header_.push_back(TypeInfo(Type::STRING, str.length(), str.data()));
					}
				}
				else
				{
					write(*vector.data());
				}
				header_.push_back(TypeInfo(Type::VECTOR | Type::END, 0));
				return *this;
			}

			Writer& write(std::string& str)
			{
				printf("write string\n");
				header_.push_back(TypeInfo(Type::STRING, str.length(), reinterpret_cast<const char*>(std::addressof(str))));
				return *this;
			}

			template<typename T>
				requires IsBool<T>
			Writer& write(T& value)
			{
				addPrimitiveType(Type::BOOL, value);
				return *this;
			}

			template<typename T>
				requires IsNumeric<T>
			Writer& write(T& value)
			{
				if constexpr (std::is_same<T, uint8_t>::value)
				{
					addPrimitiveType(Type::U8, value);
				}
				else if constexpr (std::is_same<T, uint16_t>::value)
				{
					addPrimitiveType(Type::U16, value);
				}
				else if constexpr (std::is_same<T, uint32_t>::value)
				{
					addPrimitiveType(Type::U32, value);
				}
				else if constexpr (std::is_same<T, uint64_t>::value)
				{
					addPrimitiveType(Type::U64, value);
				}
				else if constexpr (std::is_same<T, int8_t>::value)
				{
					addPrimitiveType(Type::I8, value);
				}
				else if constexpr (std::is_same<T, int16_t>::value)
				{
					addPrimitiveType(Type::I16, value);
				}
				else if constexpr (std::is_same<T, int32_t>::value)
				{
					addPrimitiveType(Type::I32, value);
				}
				else if constexpr (std::is_same<T, int64_t>::value)
				{
					addPrimitiveType(Type::I64, value);
				}
				else if constexpr (std::is_same<T, float>::value)
				{
					addPrimitiveType(Type::F32, value);
				}
				else if constexpr (std::is_same<T, double>::value)
				{
					addPrimitiveType(Type::F64, value);
				}
				else
				{
					throw TraceException("Cannot write an unknown type!");
				}
				return *this;
			}

			template<typename T>
				requires IsClass<T>
			Writer& write(T& value)
			{
				if constexpr (std::is_class<T>::value)
				{
					header_.emplace_back(Type::STRUCT, sizeof(T), reinterpret_cast<char*>(std::addressof(value)));
				}
				else
				{
					throw TraceException("Cannot write an unknown type!");
				}
				return *this;
			}

			// template<typename T>
			// Writer& write(T value)
			// {
			// 	write(value);
			// 	return *this;
			// }

			void flush();

		private:
			void flushHeader(std::ofstream& os);
			std::size_t flushVector(std::ofstream& os, std::size_t index, std::size_t size);

			Header header_;
			std::string file_;

		};

		struct Parser
		{
		public:
			Parser(std::ifstream& is);

			inline const Header& getHeader() { return header_; }

		private:
			void read(char* ptr);
			std::size_t readVector(char* ptr, std::size_t nextIndex, std::size_t size);

		public:
			template<typename T>
			void read(T& data)
			{
				read(reinterpret_cast<char*>(std::addressof(data)));
			}

		private:
			std::ifstream& is_;
			std::size_t infoIndex_;
			Header header_;
		};

		class Reader
		{
		public:
			Reader(std::string file);
			Reader(const Reader&) = delete;
			Reader(Reader&&) = delete;

			template<typename T>
			void read(T callback)
			{
				std::ifstream is = std::ifstream(file_, std::ifstream::beg | std::ifstream::binary);
				Parser parser(is);
				callback(parser);
			}

		private:
			std::string file_;
		};
	};
}