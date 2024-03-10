#pragma once

#include <Engine/Core/Assert.h>
#include <Engine/Core/PlatformDetection.h>

#include <memory>
#include <exception>
#include <filesystem>

#define BIT(x) (1 << x)

namespace dyxide
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	static std::string GetCurrentDir()
	{
		try
		{
			return std::filesystem::current_path().string();
		}
		catch (const std::exception& e)
		{
			DYXIDE_ASSERT(false, "Failed to retrieve current directory: " + std::string(e.what()));
			return "";
		}
	}

	static std::string GetResourceDir()
	{
		std::string currentDir = GetCurrentDir();
		if (!currentDir.empty())
		{
			return currentDir + "/resources/";
		}
		else
		{
			DYXIDE_ASSERT(false, "Failed to retrieve resource directory");
			return "";
		}
	}
}