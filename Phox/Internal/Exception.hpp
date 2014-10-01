#ifndef __PhoxEngineException__
#define __PhoxEngineException__ 1
#include <exception>
#include <string>
#include <iostream>
#include <cstdlib>

namespace Phox
{
    ////////////////////////////////////////////////////////////
    /// \brief Class for throwing exceptions.
    ////////////////////////////////////////////////////////////
    class Exception: public std::exception
    {
        public:
            ////////////////////////////////////////////////////////////
            /// \brief Defualt constructor.
            ///
            /// This constructor will throw a generic exception.
            ///
            ////////////////////////////////////////////////////////////
            inline explicit Exception()
            {
                Error = "Exception: Generic Exception Thrown";
            }

            ////////////////////////////////////////////////////////////
            /// \brief Constructor.
            ///
            /// This constructor will throw an exception with the given
            /// text.
            ///
            ////////////////////////////////////////////////////////////
            inline Exception(const std::string& String)
            {
                Error = "Exception: ";
                Error += String;
            }

            ////////////////////////////////////////////////////////////
            /// \brief Override of the what() std::exception function.
            ///
            /// To be used internally to get the exception string.
            ///
            ////////////////////////////////////////////////////////////
            inline virtual const char* what() const throw()
            {
                return Error.c_str();
            }

        private:
            std::string Error;///< The error string.
    };

    ////////////////////////////////////////////////////////////
    /// \brief Throw a Phox exception.
    ///
    /// This function will create an instance of a Phox exception
    /// which is like a standard exception, but has the ability
    /// to stop execution of the program. This function also
    /// automatically adds the exception to the Debugger.
    ///
    /// \param String Message of the exception.
    /// \param Exit Whether or not to stop execution of the program.
    ////////////////////////////////////////////////////////////
    void ThrowException(const std::string& String, bool Exit = 0, std::ostream& Out = std::cerr);
}
#endif
////////////////////////////////////////////////////////////
/// \class Phox::Exception
///
/// This class can be instantiated, however it's better used
/// internally by calling ThrowException();
///
/// Usage example:
/// \code
/// if (Something) {}
/// else
/// {
///      ThrowException("Something Failed! Aborting program.", true);
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
