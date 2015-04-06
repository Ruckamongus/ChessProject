#ifndef __GenericBufferH__
#define __GenericBufferH__ 1
#include <string>

namespace Phox
{
    class cGenericBuffer
    {
        public:
            cGenericBuffer()=default;
            cGenericBuffer(char* Source) {setBasePointer(Source);}
            virtual ~cGenericBuffer()=default;
            cGenericBuffer(const cGenericBuffer& other)=default;
            cGenericBuffer(cGenericBuffer&& other)=default;
            cGenericBuffer& operator=(const cGenericBuffer& other)=default;
            cGenericBuffer& operator=(cGenericBuffer&& other)=default;

            inline std::size_t getWritePosition()  const {return m_WritePosition;}
            inline std::size_t getReadPosition()   const {return m_ReadPosition;}
            inline char*       getPointer()        const {return m_BasePointer;}
            inline const char* getConstPointer()   const {return m_BasePointer;}
            inline void setWritePosition(std::size_t Position) {m_WritePosition = Position;}
            inline void setReadPosition (std::size_t Position) {m_ReadPosition  = Position;}
            inline bool setBasePointer(char* Address) {if (Address == nullptr) return 0; m_BasePointer = Address; return 1;}

            virtual bool isValid() const {return m_BasePointer != nullptr;}
            virtual void clear() {m_ReadPosition = 0; m_WritePosition = 0;}
            virtual bool write(const void* Source, std::size_t Count);
            virtual bool read (void* Destination, std::size_t Count);

            bool writeByte          (signed char Data)    {return write(&Data, sizeof(Data));}
            bool writeUnsignedByte  (unsigned char Data)  {return write(&Data, sizeof(Data));}
            bool writeShort         (signed short Data)   {return write(&Data, sizeof(Data));}
            bool writeUnsignedShort (unsigned short Data) {return write(&Data, sizeof(Data));}
            bool writeInt           (signed int Data)     {return write(&Data, sizeof(Data));}
            bool writeUnsignedInt   (unsigned int Data)   {return write(&Data, sizeof(Data));}
            bool writeLong          (signed long Data)    {return write(&Data, sizeof(Data));}
            bool writeUnsignedLong  (unsigned long Data)  {return write(&Data, sizeof(Data));}
            bool writeFloat         (float Data)          {return write(&Data, sizeof(Data));}
            bool writeDouble        (double Data)         {return write(&Data, sizeof(Data));}
            bool writeString        (const char* String)  {return write(String, getNullChar(String));}
            bool writeString        (const std::string& String) {return writeString(String.c_str());}


            signed char     readByte()           {signed char Data = 0; read(&Data, sizeof(Data)); return Data;}
            unsigned char   readUnsignedByte()   {unsigned char Data = 0; read(&Data, sizeof(Data)); return Data;}
            signed short    readShort()          {signed short Data = 0; read(&Data, sizeof(Data)); return Data;}
            unsigned short  readUnsignedShort()  {unsigned short Data = 0; read(&Data, sizeof(Data)); return Data;}
            signed int      readInt()            {signed int Data = 0; read(&Data, sizeof(Data)); return Data;}
            unsigned int    readUnsignedInt()    {unsigned int Data = 0; read(&Data, sizeof(Data)); return Data;}
            long            readLong()           {signed long Data = 0; read(&Data, sizeof(Data)); return Data;}
            unsigned long   readUnsignedLong()   {unsigned long Data = 0; read(&Data, sizeof(Data)); return Data;}
            float           readFloat()          {float Data = 0.f; read(&Data, sizeof(Data)); return Data;}
            double          readDouble()         {double Data = 0; read(&Data, sizeof(Data)); return Data;}
            std::string     readString();

            cGenericBuffer& operator<< (signed char Data)     {writeByte(Data); return *this;}
            cGenericBuffer& operator<< (unsigned char Data)   {writeUnsignedByte(Data); return *this;}
            cGenericBuffer& operator<< (signed short Data)    {writeShort(Data); return *this;}
            cGenericBuffer& operator<< (unsigned short Data)  {writeUnsignedShort(Data); return *this;}
            cGenericBuffer& operator<< (signed int Data)      {writeInt(Data); return *this;}
            cGenericBuffer& operator<< (unsigned int Data)    {writeUnsignedInt(Data); return *this;}
            cGenericBuffer& operator<< (signed long Data)     {writeLong(Data); return *this;}
            cGenericBuffer& operator<< (unsigned long Data)   {writeUnsignedLong(Data); return *this;}
            cGenericBuffer& operator<< (float Data)           {writeFloat(Data); return *this;}
            cGenericBuffer& operator<< (double Data)          {writeDouble(Data); return *this;}
            cGenericBuffer& operator<< (const std::string& Data)    {writeString(Data); return *this;}

            cGenericBuffer& operator>> (signed char& Data)    {Data = readByte(); return *this;}
            cGenericBuffer& operator>> (unsigned char& Data)  {Data = readUnsignedByte(); return *this;}
            cGenericBuffer& operator>> (signed short& Data)   {Data = readShort(); return *this;}
            cGenericBuffer& operator>> (unsigned short& Data) {Data = readUnsignedShort(); return *this;}
            cGenericBuffer& operator>> (signed int& Data)     {Data = readInt(); return *this;}
            cGenericBuffer& operator>> (unsigned int& Data)   {Data = readUnsignedInt(); return *this;}
            cGenericBuffer& operator>> (signed long& Data)    {Data = readLong(); return *this;}
            cGenericBuffer& operator>> (unsigned long& Data)  {Data = readUnsignedLong(); return *this;}
            cGenericBuffer& operator>> (float& Data)          {Data = readFloat(); return *this;}
            cGenericBuffer& operator>> (double& Data)         {Data = readDouble(); return *this;}
            cGenericBuffer& operator>> (std::string& Data)    {Data = readString(); return *this;}

        protected:
            char*        m_BasePointer = nullptr;
            unsigned int m_ReadPosition = 0;
            unsigned int m_WritePosition = 0;

        private:
            inline unsigned int getNullChar(const char* String) const
            {
                unsigned int Count = 0;
                char CurrentChar = ' ';
                while (CurrentChar != '\0')
                {
                    CurrentChar = String[Count];
                    Count++;
                }

                return Count;
            }
    };
}

#endif
