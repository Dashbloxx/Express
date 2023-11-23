#pragma once

#include <vector>

namespace VarInt
{
	int Encode(std::uint64_t Value, std::vector<std::uint8_t>& Vector);
	int Decode(std::vector<std::uint8_t>& Vector, std::uint64_t& Value);
	int GetEncodedSize(std::size_t &Size, std::size_t Value);
};