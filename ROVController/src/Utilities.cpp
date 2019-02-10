#include "Utilities.h"

unsigned GetSeed(const std::string& seed) {
    return static_cast<unsigned>(std::hash<std::string>{}(seed));
}

void QuitWithError(const char* error, int exit_code)
{
#ifdef _WIN32
	MessageBox(nullptr, error, "City Generator Runtime Error", MB_OK | MB_ICONINFORMATION);
#else
#include <iostream>
    std::cerr<< "City Generator Runtime Error" << std::endl;
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
