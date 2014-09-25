#ifndef __PhoxConversionsHPP__
#define __PhoxConversionsHPP__ 1
#include <sstream>
#include <cstdlib>

namespace Phox
{
    template <class T>
    inline std::string toString(const T& Argument)
    {
        std::stringstream Out;
        Out << Argument;
        return Out.str();
    }

    inline double ToDouble(const std::string& Argument)
    {
        char* EndPtr;
        return std::strtod(Argument.c_str(), &EndPtr);
    }
}
#endif
