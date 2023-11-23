#include <vector>
#include <string>

#include "ipc.hpp"
#include "leb128.hpp"

int InterProcessCommunication::Encode(std::string String, std::vector<std::uint8_t> &Vector)
{
	std::size_t StringSize = String.size();

	VarInt::Encode(StringSize, Vector);

	for(char Character : String)
	{
		Vector.push_back(Character);
	}

	return 0;
}

int InterProcessCommunication::Decode(std::vector<std::uint8_t> &Vector, std::string& String)
{
    std::size_t StringSize;
    VarInt::Decode(Vector, StringSize);

    String.clear();

    for (std::size_t i = 0; i < StringSize; ++i)
    {
        std::size_t Size;
        VarInt::GetEncodedSize(Size, StringSize);
        String.push_back(Vector[i + Size]);
    }

    return 0;
}