#include <Phox/Internal/Exception.hpp>

namespace Phox
{
    void ThrowException(const std::string& String, bool Abort)
    {
        std::string Error = "[Phox Exception] ";
        Error += String;

        try
        {
            throw Exception(Error);
        }
        catch (std::exception& E)
        {
            E.what();
        }

        if (Abort)
        {
            std::abort();
        }
    }

    void ThrowException(const std::string& String, std::ostream& Out, bool Abort)
    {
        std::string Error = "[Phox Exception] ";
        Error += String;

        Out << Error << '\n';

        try
        {
            throw Exception(Error);
        }

        catch (std::exception& E)
        {
            E.what();
        }

        if (Abort)
        {
            std::abort();
        }
    }
}
