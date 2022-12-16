#include "pch.hpp"
#include "Utils.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	namespace Utils
	{
		const std::wstring toWString(const char* str)
		{
			std::size_t length = strlen(str) + 1;
			std::wstring wc(length, L'\0');
			std::size_t numOfConverted = 0;
			mbstowcs_s(&numOfConverted, &wc[0], length, str, length);
			return wc;
		}

		void formatPath(std::string& path)
		{
			formatPath(path.data());
		}

		void formatPath(char* path)
		{
			char c;
			std::size_t i = 0;
			while ((c = path[i++]) != '\0')
			{
				if (c == '\\')
					path[i - 1] = '/';
			}
		}

		std::size_t getCurrentThreadID()
		{
#ifdef _WINDOWS
			return GetCurrentThreadId();
#endif
			return 0;
		}

		std::string getExecPathNative()
		{
#ifdef _WIN32
			char pBuf[256];
			DWORD len = static_cast<DWORD>(sizeof(pBuf));

			DWORD bytes = GetModuleFileNameA(NULL, pBuf, len);
			if(!bytes)
				throw TraceException("getExecutablePath is not implemented yet!");
			return std::string(pBuf);

#elif
			throw TraceException("getExecutablePath is not implemented yet!");
#endif
		}

		const std::filesystem::path& getExecutablePath()
		{
			static std::filesystem::path execPath = getExecPathNative();
			return execPath;
		}
	}
}