#pragma once

#include <source_location>
#include <sstream>

namespace dyxide
{
	enum Severity
	{
		DEBUG = 0,
		INFO = 1,
		WARN = 2,
		ERROR = 3,
		CRITICAL = 4
	};

	extern void Log(const Severity severity, std::source_location const source, const std::ostringstream& oss);
}

#define DYXIDE_TRACE(...) { std::ostringstream oss; oss << __VA_ARGS__; ::dyxide::Log((::dyxide::Severity)0, std::source_location::current(), oss); }
#define DYXIDE_INFO(...) { std::ostringstream oss; oss << __VA_ARGS__; ::dyxide::Log((::dyxide::Severity)1, std::source_location::current(), oss); }
#define DYXIDE_WARN(...) { std::ostringstream oss; oss << __VA_ARGS__; ::dyxide::Log((::dyxide::Severity)2, std::source_location::current(), oss); }
#define DYXIDE_ERROR(...) { std::ostringstream oss; oss << __VA_ARGS__; ::dyxide::Log((::dyxide::Severity)3, std::source_location::current(), oss); }
#define DYXIDE_CRITICAL(...) { std::ostringstream oss; oss << __VA_ARGS__; ::dyxide::Log((::dyxide::Severity)4, std::source_location::current(), oss); }