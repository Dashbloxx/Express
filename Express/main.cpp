#include <vector>
#include <string>
#include <iostream>

#include "ipc.hpp"

int main(int argc, const char *argv[])
{
	std::vector<std::uint8_t> Vector;
	std::string String0 = "Hello, world!";
	std::string String1;

	InterProcessCommunication::Encode(String0, Vector);
	InterProcessCommunication::Decode(Vector, String1);

	std::cout << String1 << std::endl;

	return 0;
}