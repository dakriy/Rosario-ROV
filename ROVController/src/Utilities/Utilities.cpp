#include "Utilities.h"
#include "../Core/GlobalContext.h"
#include <imgui.h>
#include <ctime>

unsigned GetSeed(const std::string& seed) {
    return static_cast<unsigned>(std::hash<std::string>{}(seed));
}

void QuitWithError(const char* error, int exit_code)
{
#ifdef _WIN32
	MessageBox(nullptr, error, "ROV Controller Runtime Error", MB_OK | MB_ICONINFORMATION);
#else
#include <iostream>
    std::cerr<< error << std::endl;
#endif//_WIN32
	exit(exit_code);
}

int GetRandomNumberInRange(int min, int max)
{
	static auto __rand_first{ true };
	if (__rand_first) {
		__rand_first = false;
		rnd.seed(std::random_device()());
	}

	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
	return dist(rnd);
}

/*
void copyScreenshotToClipboard(const sf::Image & image)
{
	BITMAPINFOHEADER bmpInfoHeader = {0};
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = image.getSize().y;
	bmpInfoHeader.biWidth = image.getSize().x;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = image.getSize().x * image.getSize().y * 3;

	HGLOBAL hResult;
	if (!OpenClipboard(NULL)) return ;
	if (!EmptyClipboard()) return ;

	int size = sizeof(BITMAPINFOHEADER) + bmpInfoHeader.biSizeImage;
	hResult = GlobalAlloc(GMEM_MOVEABLE, size);
	if (hResult == NULL) return ;

	void * pointer = GlobalLock(hResult);
	memcpy(pointer, &bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	pointer = (void*) ((int)pointer + sizeof(BITMAPINFOHEADER));
	const unsigned char * src = image.getPixelsPtr() + (image.getSize().y - 1) * image.getSize().x * 4;
	unsigned char * dest = (unsigned char*)pointer;
	unsigned int srcIndex = 0;
	unsigned int dstIndex = 0;
	for (unsigned int i = 0; i < image.getSize().y; i++) {
		for (unsigned int j = 0; j < image.getSize().x; j++) {
			dest[dstIndex + 0] = src[srcIndex + 2];
			dest[dstIndex + 1] = src[srcIndex + 1];
			dest[dstIndex + 2] = src[srcIndex + 0];
			dstIndex += 3;
			srcIndex += 4;
		}
		srcIndex = 0;
		src -= image.getSize().x * 4;
		if (image.getSize().x % 4 != 0) dstIndex += 4 - image.getSize().x % 4;
	}

	GlobalUnlock(hResult);

	if (SetClipboardData(CF_DIB, hResult) == NULL) {
		CloseClipboard();
		return ;        // error
	}

	CloseClipboard();
	GlobalFree(hResult);
}*/

sf::Vector2f convertToScreenCoords(sf::Rect<double> bounds, sf::Vector2<double> coords) {
    auto windowSize = GlobalContext::get_window()->getSize();
    auto scaleX = windowSize.x / bounds.width;
    auto scaleY = windowSize.y / bounds.height;
    return sf::Vector2f(static_cast<float>((coords.x - bounds.left) * scaleX), static_cast<float>((bounds.top - coords.y) * scaleY));
}

sf::Vector2<double> convertToLocalCoords(sf::Rect<double> bounds, sf::Vector2<double> screenCoords) {
    auto windowSize = GlobalContext::get_window()->getSize();
    auto scaleX = bounds.width / windowSize.x;
    auto scaleY = bounds.height / windowSize.y;
    return sf::Vector2<double>(static_cast<double>(screenCoords.x * scaleX), static_cast<double>(screenCoords.y * scaleY));
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see misc/fonts/README.txt)
void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

//int tm_sec;   // seconds after the minute - [0, 60] including leap second
//int tm_min;   // minutes after the hour - [0, 59]
//int tm_hour;  // hours since midnight - [0, 23]
//int tm_mday;  // day of the month - [1, 31]
//int tm_mon;   // months since January - [0, 11]
//int tm_year;  // years since 1900
//int tm_wday;  // days since Sunday - [0, 6]
//int tm_yday;  // days since January 1 - [0, 365]
//int tm_isdst; // daylight savings time flag
std::string currentDateTime() {
	std::time_t t = std::time(nullptr);   // get time now
	std::tm* now = std::localtime(&t);
	std::string timeStr = std::to_string(now->tm_year + 1900) + '-'
			  + std::to_string(now->tm_mon + 1) + '-'
			  + std::to_string(now->tm_mday) + '-'
			  + std::to_string(now->tm_hour) + '-'
			  + std::to_string(now->tm_min) + '-'
			  + std::to_string(now->tm_sec);
	return timeStr;
}
