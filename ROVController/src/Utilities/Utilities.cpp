#include "Utilities.h"

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
}
