#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	template <class T>
	concept IsStringLike = std::is_convertible_v<T, std::string_view>;

	template <typename T>
	concept IsLoggable = requires(T loggable)
	{
		{ loggable.log() } -> IsStringLike;
	};

	template<typename T>
	concept IsNumeric = requires(T param)
	{
		requires std::is_integral_v<T> || std::is_floating_point_v<T>;
		requires !std::is_same_v<bool, T>;
		requires std::is_arithmetic_v<decltype(param + 1)>;
		requires !std::is_pointer_v<T>;
	};

	template <class T> // concept
	concept IsClass = std::is_class<T>::value;

	template<typename T>
	concept IsBool = std::is_same_v<bool, T>;
}