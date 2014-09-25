#ifndef __PhoxEngineException__
#define __PhoxEngineException__ 1
#include <exception>
#include <string>
#include <ostream>
#include <cstdlib>
#ifdef PHOX_DEBUG_MODE_ENABLED
#include <Phox/Utilities/DebugLog.hpp>
#endif // PHOX_DEBUG_MODE_ENABLED

namespace Phox
{
    #ifdef PHOX_DEBUG_MODE_ENABLED
        extern cDebugLog Debugger;
    #endif

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
            #ifdef PHOX_ENABLE_INLINES
            inline
            #endif
            explicit Exception() {Error = "Phox Exception: Generic Exception Thrown";}

            ////////////////////////////////////////////////////////////
            /// \brief Constructor.
            ///
            /// This constructor will throw an exception with the given
            /// text.
            ///
            ////////////////////////////////////////////////////////////
            #ifdef PHOX_ENABLE_INLINES
            inline
            #endif
            Exception(const std::string& String) {Error = "Phox Exception: "; Error += String;}

            ////////////////////////////////////////////////////////////
            /// \brief Override of the what() std::exception function.
            ///
            /// To be used internally to get the exception string.
            ///
            ////////////////////////////////////////////////////////////
            #ifdef PHOX_ENABLE_INLINES
            inline
            #endif
            virtual const char* what() const throw() {return Error.c_str();}

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
    #ifdef PHOX_ENABLE_INLINES
        inline void ThrowException(const std::string& String, bool Exit = 0)
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
    #else
        void ThrowException(const std::string& String, bool Exit = 0);
    #endif

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
    #ifdef PHOX_ENABLE_INLINES
        inline void ThrowException(const std::string& String, std::ostream& Out, bool Exit = 0)
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
    #else
        void ThrowException(const std::string& String, std::ostream& Out, bool Exit = 0);
    #endif
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
