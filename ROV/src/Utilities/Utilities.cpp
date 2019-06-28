#include <iterator>
#include "Utilities.h"

std::vector<uint8_t> yeetFileIntoByteVector(std::ifstream & file)
{
	// Stop eating new lines in binary mode!!!
	file.unsetf(std::ios::skipws);
	// get its size:
	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// reserve capacity
	std::vector<uint8_t> vec;
	vec.reserve(static_cast<unsigned>(fileSize));

	// read the data:
	vec.insert(vec.begin(),
			   std::istream_iterator<uint8_t>(file),
			   std::istream_iterator<uint8_t>());

	return vec;
}
