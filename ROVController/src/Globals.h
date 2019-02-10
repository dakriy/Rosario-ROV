#pragma once
// Uncomment the next line to disable assert statments
// #define NDEBUG
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif//_WIN32

#define EVENT_FUNC_TYPE std::function<bool(const sf::Event*)>
#define EVENT_FUNC_INDEX const Core::EventHook*
#define let const auto
#define var auto

constexpr auto APP_NAME = "ROV Controller";
