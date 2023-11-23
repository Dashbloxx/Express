#include <vector>

#include "leb128.hpp"

int VarInt::Encode(std::uint64_t Value, std::vector<uint8_t> &Vector)
{
    do
    {
        std::uint8_t Byte = Value & 0x7F;
        Value >>= 7;

        if(Value != 0)
        {
            Byte |= 0x80;
        }

        Vector.push_back(Byte);
    }
    while (Value != 0);

    return 0;
}

int VarInt::Decode(std::vector<std::uint8_t> &Vector, std::uint64_t &Value)
{
    int Shift = 0;

    for(std::size_t i = 0; i < Vector.size(); ++i)
    {
        Value |= static_cast<std::uint64_t>(Vector[i] & 0x7F) << Shift;
        Shift += 7;

        if((Vector[i] & 0x80) == 0)
        {
            break;
        }
    }

    return 0;
}

int VarInt::GetEncodedSize(std::size_t &Size, std::size_t Value)
{
    Size = 0;

    do
    {
        Value >>= 7;
        Size++;
    }
    while (Value != 0);

    return 0;
}