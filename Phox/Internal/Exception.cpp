#include <Phox/Internal/Exception.hpp>

namespace Phox
{
    #ifndef PHOX_ENABLE_INLINES
        void ThrowException(const std::string& String, bool Exit)
        {
            std::string Error = "[Phox Exception] ";
            Error += String;

            #ifdef PHOX_DEBUG_MODE_ENABLED
                Debugger.addMessage(Error);
            #endif

            try {throw Exception(String);}
            catch (std::exception& E) {E.what();}
            if (Exit) abort();
        }

        void ThrowException(const std::string& String, std::ostream& Out, bool Exit)
        {
            std::string Error = "[Phox Exception] ";
            Error += String;

            #ifdef PHOX_DEBUG_MODE_ENABLED
                Debugger.addMessage(Error);
            #endif

            Out << Error << '\n';

            try {throw Exception(String);}
            catch (std::exception& E) {E.what();}
            if (Exit) abort();
        }
    #endif
}
