#pragma once
// Uncomment the next line to disable assert statments
// #define NDEBUG
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif//_WIN32

#define EVENT_FUNC_TYPE(T) std::function<bool(const T*)>
#define EVENT_FUNC_INDEX(T, S) const Core::EventHook<T, S>*
#define CORE_EVENT_FUNC_INDEX EVENT_FUNC_INDEX(Core::Event, Core::Event::EventType::Count)
#define let const auto
#define var auto

constexpr double E = 2.7182818284590452353602874713527;

// Interestingly enough, NASA only uses about 15 digits of PI for spaceflight.
constexpr double PI = 3.1415926535897932384626433832;

constexpr auto APP_NAME = "ROV Controller";
