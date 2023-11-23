#pragma once

#include <vector>
#include <string>

namespace InterProcessCommunication
{
	int Encode(std::string String, std::vector<std::uint8_t>& Vector);
	int Decode(std::vector<std::uint8_t> &Vector, std::string& String);
};