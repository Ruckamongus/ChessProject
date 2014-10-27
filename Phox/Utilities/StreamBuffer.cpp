#include <Phox/Utilities/StreamBuffer.hpp>
#include <cstdlib>
#include <cstring>

namespace Phox
{
    cStreamBuffer::cStreamBuffer()
    {
        m_BasePointer = static_cast<char*> (std::malloc(sizeof(char) * m_BufferResize));
        if (m_BasePointer != nullptr)
        {
            m_AllocatedSize = m_BufferResize;
        }
    }

    cStreamBuffer::cStreamBuffer(std::size_t DefaultSize)
    {
        m_BufferResize = DefaultSize;
        m_BasePointer = static_cast<char*> (std::malloc(sizeof(char) * m_BufferResize));
        if (m_BasePointer != nullptr)
        {
            m_AllocatedSize = m_BufferResize;
        }
    }

    cStreamBuffer::cStreamBuffer(const cStreamBuffer& Original) : cGenericBuffer()
    {
        if (m_AllocatedSize != Original.m_AllocatedSize)
        {
            if (m_BasePointer != nullptr) std::free(m_BasePointer);
            m_BasePointer = static_cast<char*> (std::malloc(sizeof(char) * Original.getAllocatedBytes()));
        }

        if (m_BasePointer != nullptr)
        {
            std::memcpy(m_BasePointer, Original.m_BasePointer, Original.getAllocatedBytes());
            m_ReadPosition  = Original.m_ReadPosition;
            m_WritePosition = Original.m_WritePosition;
            m_AllocatedSize = Original.m_AllocatedSize;
            m_WorkingSize   = Original.m_WorkingSize;
            m_BufferResize  = Original.m_BufferResize;
        }
    }

    cStreamBuffer::cStreamBuffer(cStreamBuffer&& Original)
    {
        std::swap(m_ReadPosition, Original.m_ReadPosition);
        std::swap(m_WritePosition, Original.m_WritePosition);
        std::swap(m_AllocatedSize, Original.m_AllocatedSize);
        std::swap(m_WorkingSize, Original.m_WorkingSize);
        std::swap(m_BufferResize, Original.m_BufferResize);
        std::swap(m_BasePointer, Original.m_BasePointer);
    }

    cStreamBuffer::~cStreamBuffer()
    {
        if (m_BasePointer != nullptr)
            std::free(m_BasePointer);
    }

    void cStreamBuffer::clear()
    {
        m_WritePosition = 0;
        m_ReadPosition = 0;
        m_WorkingSize = 0;
    }

    cStreamBuffer& cStreamBuffer::operator= (const cStreamBuffer& Original)
    {
        if (*this == Original) return *this;
        m_BasePointer = static_cast<char*> (std::malloc(sizeof(char) * Original.getAllocatedBytes()));

        if (m_BasePointer != nullptr)
        {
            std::memcpy(m_BasePointer, Original.m_BasePointer, Original.getAllocatedBytes());
            m_ReadPosition  = Original.m_ReadPosition;
            m_WritePosition = Original.m_WritePosition;
            m_AllocatedSize = Original.m_AllocatedSize;
            m_WorkingSize   = Original.m_WorkingSize;
            m_BufferResize  = Original.m_BufferResize;
        }

        return *this;
    }

    cStreamBuffer& cStreamBuffer::operator+=(const cStreamBuffer& Other)
    {
        write(Other.getPointer(), Other.getWorkingBytes());
        return *this;
    }

    bool cStreamBuffer::operator== (const cStreamBuffer& Other) const
    {
        if (&Other == this) return true;
        if (Other.m_WorkingSize != this->m_WorkingSize) return false;
        for (std::size_t i = 0; i <= m_WorkingSize; i++)
        {
            if (Other.m_BasePointer[i] != this->m_BasePointer[i])
                return false;
        }
        return true;
    }

    bool cStreamBuffer::write(const void* Source, std::size_t Count)
    {
        if (Count + m_WorkingSize >= m_AllocatedSize)
        {
            void* TempPointer = std::realloc(m_BasePointer, m_AllocatedSize + Count + m_BufferResize);
            if (TempPointer == nullptr) return false;

            m_BasePointer = static_cast<char*> (TempPointer);
            m_AllocatedSize += (Count + m_BufferResize);
        }

        std::memcpy(m_BasePointer + m_WritePosition, Source, Count);
        m_WritePosition += Count;
        if (m_WritePosition > m_WorkingSize) m_WorkingSize = m_WritePosition;
        return true;
    }

    bool cStreamBuffer::read(void* Source, std::size_t Count)
    {
        if ((m_ReadPosition + Count) > m_AllocatedSize) return false;
        if (Count == 0) return false;
        std::memcpy(Source, m_BasePointer + m_ReadPosition, Count);
        m_ReadPosition += Count;
        return true;
    }

    char cStreamBuffer::operator[] (std::size_t Position) const
    {
        if (Position >= m_AllocatedSize) return '\0';
        return m_BasePointer[Position];
    }

    bool cStreamBuffer::allocate(std::size_t Bytes)
    {
        if (Bytes <= m_WorkingSize) return false;
        void* TempPointer = std::realloc(m_BasePointer, Bytes + m_BufferResize);
        if (TempPointer == nullptr) return false;

        m_BasePointer = static_cast<char*> (TempPointer);
        m_AllocatedSize = Bytes + m_BufferResize;
        m_WorkingSize = Bytes;
        return true;
    }
}
