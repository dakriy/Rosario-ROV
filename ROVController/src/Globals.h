#pragma once
// Uncomment the next line to disable assert statments
// #define NDEBUG
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif//_WIN32

#define EVENT_FUNC_TYPE(T) std::function<bool(const T*)>
#define EVENT_FUNC_INDEX(T, S) const Core::EventHook<T, S>*
#define let const auto
#define var auto

constexpr auto APP_NAME = "ROV Controller";
