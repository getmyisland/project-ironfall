#pragma once

#include <Engine/Core/Log.h>

#ifndef NDEBUG
#   define DYXIDE_ASSERT(condition, ...) \
    do { \
        if (!(condition)) { \
            DYXIDE_ERROR("Assertion failed: " << condition) \
            DYXIDE_ERROR(__VA_ARGS__) \
            std::terminate(); \
        } \
    } while (false)
#else
#   define DYXIDE_ASSERT(condition, ...) do { } while (false)
#endif
