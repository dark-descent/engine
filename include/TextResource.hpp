#pragma once

#include "pch.hpp"
#include "Resource.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	class TextResource: public Resource<TextResource, std::string>
	{
	public:
		TextResource(const std::string& path): Resource(path) { };
		virtual ~TextResource() { delete data_; };

		virtual void onLoad()
		{
			std::filesystem::path p(path);

			if (p.is_relative())
				p = Engine::getInstance().gamePath / path;

			if (!std::filesystem::exists(p))
			{
				auto msg = std::format("{} does not exists!", p.string().c_str());
				throw TraceException(msg.c_str());
			}

			std::ifstream is(p);
			data_ = new std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		}
	};
}