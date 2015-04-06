#ifndef __StreamBufferH__
#define __StreamBufferH__ 1
#include <Phox/Utilities/GenericBuffer.hpp>

namespace Phox
{
    class cStreamBuffer: public cGenericBuffer
    {
        public:
            explicit cStreamBuffer();
                     cStreamBuffer(std::size_t DefaultSize);
                     cStreamBuffer(const cStreamBuffer& Original);
                     cStreamBuffer(cStreamBuffer&& Original);
            virtual ~cStreamBuffer();


            bool setBasePointer(char* Address) = delete;

            unsigned int getAllocatedBytes()   const {return m_AllocatedSize;}
            unsigned int getWorkingBytes()     const {return m_WorkingSize;}

            bool allocate(std::size_t Bytes);

            virtual void clear();
            virtual bool write(const void* Source, std::size_t Count);
            virtual bool read (void* Destination, std::size_t Count);

            cStreamBuffer& operator=   (const cStreamBuffer& Original);
            cStreamBuffer& operator=   (cStreamBuffer&& other);
            cStreamBuffer& operator+=  (const cStreamBuffer& Other);
            bool           operator==  (const cStreamBuffer& Other) const;
            char           operator[]  (std::size_t) const;
            inline         operator char*() const {return m_BasePointer;}

        private:
            unsigned int m_AllocatedSize = 0;
            unsigned int m_WorkingSize = 0;
            unsigned int m_BufferResize = 30;
    };
}

#endif
