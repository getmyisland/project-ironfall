#pragma once

#include "Engine/Core/PlatformDetection.h"

#ifdef DYXIDE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <cstdint>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Engine/Core/Common.h"
#include "Engine/Core/Log.h"

#ifdef DYXIDE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif