#pragma once

#include <imgui.h>
#include <mutex>

class AppLog {
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool                AutoScroll;
	bool                ScrollToBottom;
	std::mutex			protection;
public:

	AppLog();
	void Clear();
	void AddLog(const char * fmt, ...) IM_FMTARGS(2);
	void Draw(const char * title, bool* p_open = nullptr);
};
