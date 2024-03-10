#include <Engine/Core/Log.h>

#include <chrono>
#include <string>
#include <iostream>
#include <filesystem>

namespace dyxide
{
	static std::string GetCurrentTime()
	{
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		std::string time_str = std::ctime(&time);
		time_str.pop_back();

		return time_str;
	}

	static std::string GetLocationString(std::source_location const source)
	{
		return "[" + std::filesystem::path(source.file_name()).filename().string() + "] [" + source.function_name() + "] [" + std::to_string(source.line()) + ":" + std::to_string(source.column()) + "]";
	}

	static std::string GetSeverityString(const Severity severity)
	{
		switch (severity)
		{
		default:
		case Severity::DEBUG:
			return "TRACE";
		case Severity::INFO:
			return "INFO";
		case Severity::WARN:
			return "WARN";
		case Severity::ERROR:
			return "ERROR";
		case Severity::CRITICAL:
			return "CRITICAL";
		}
	}

	void Log(const Severity severity, std::source_location const source, const std::ostringstream& oss)
	{
		std::ostringstream poss;
		poss << GetCurrentTime() << " " << GetSeverityString(severity) << " " << GetLocationString(source) << " ";
		poss << oss.str();
		poss << std::endl;
		std::cout << poss.str();
	}
}