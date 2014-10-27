#include <Phox/Utilities/GenericBuffer.hpp>
#include <cstring>

namespace Phox
{
    std::string cGenericBuffer::readString()
    {
        char Char = ' ';
        std::string Return("");

        while (Char != '\0')
        {
           Char = readByte();
           if (Char != '\0') Return += Char;
        }

        return Return;
    }

    bool cGenericBuffer::write(const void* Source, std::size_t Count)
    {
        if (m_BasePointer == nullptr) return false;
        std::memcpy(m_BasePointer + m_WritePosition, Source, Count);
        m_WritePosition += Count;
        return true;
    }

    bool cGenericBuffer::read(void* Source, std::size_t Count)
    {
        if (Count == 0 || m_BasePointer == nullptr) return false;
        std::memcpy(Source, m_BasePointer + m_ReadPosition, Count);
        m_ReadPosition += Count;
        return true;
    }
}
