#ifndef DYXIDE_ASSERT_H
#define DYXIDE_ASSERT_H

#include <cassert>

#ifdef DYXIDE_ENABLE_ASSERTS

#define DYXIDE_ASSERT(condition, ...) do { if (!(condition)) { fprintf(stderr, __VA_ARGS__); assert(condition); } } while (0)

#else

#define DYXIDE_ASSERT(...)

#endif

#endif