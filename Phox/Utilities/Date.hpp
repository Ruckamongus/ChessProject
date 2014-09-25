#ifndef __PhoxEngineDate__
#define __PhoxEngineDate__ 1
#include <ctime>
#include <string>

namespace Phox
{
////////////////////////////////////////////////////////////
/// \brief Class for handling date and time functions.
/// Only a single instance of this class should exist, and
/// is created for you by Phox.
////////////////////////////////////////////////////////////
    class Date
    {
        public:
            ////////////////////////////////////////////////////////////
            /// \brief Return the current day as a string.
            ///
            /// \return A string between 0 and 31.
            ////////////////////////////////////////////////////////////
            static std::string day();

            ////////////////////////////////////////////////////////////
            /// \brief Return the current month name string.
            ///
            /// \return The name of the month.
            ////////////////////////////////////////////////////////////
            static std::string month();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the current year.
            ///
            /// \return The year as a string.
            ////////////////////////////////////////////////////////////
            static std::string year();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the name of the day of the week.
            ///
            /// \return The name of the day of the week.
            ////////////////////////////////////////////////////////////
            static std::string weekDay();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the hour as a string.
            ///
            /// \return The hour as a string.
            ////////////////////////////////////////////////////////////
            static std::string hour();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the minyte as a string.
            ///
            /// \return The minute as a string.
            ////////////////////////////////////////////////////////////
            static std::string minute();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the second as a string.
            ///
            /// \return The second as a string.
            ////////////////////////////////////////////////////////////
            static std::string second();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the day of the month as an integer.
            ///
            /// \return The day of the month as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int dayInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the month as an integer.
            ///
            /// \return The month as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int monthInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the year as an integer.
            ///
            /// \return The year as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int yearInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the day of the week as an integer.
            ///
            /// \return The day of the week as an integer. Sunday = 0
            ////////////////////////////////////////////////////////////
            static unsigned int weekDayInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the hour as an integer.
            ///
            /// \return The hour as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int hourInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the minute as an integer.
            ///
            /// \return The minute as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int minuteInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the second as an integer.
            ///
            /// \return The second as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int secondInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns the day of the year as an integer.
            ///
            /// \return The day of the year as an integer.
            ////////////////////////////////////////////////////////////
            static unsigned int yearDayInteger();

            ////////////////////////////////////////////////////////////
            /// \brief Returns daylight savings time.
            ///
            /// \return True Daylight savings time is active.
            /// \return False Daylight savings time is not active.
            ////////////////////////////////////////////////////////////
            static bool daylightSavingsTime();

        //private:
            static time_t m_RawTimeData;///< The raw time data.
            static struct std::tm* m_TimeInfo;///< The struct containing the c-time info.
            static std::string m_TimeString;///< The string with all of the time jumbled together.
            #ifdef PHOX_ENABLE_INLINES
                static inline void update()///< An internal function to update TimeString.
                {
                    std::time(&m_RawTimeData);
                    m_TimeInfo = std::localtime(&m_RawTimeData);
                    m_TimeString = std::asctime(m_TimeInfo);
                }
            #else
                static inline void update()///< An internal function to update TimeString.
                {
                    std::time(&m_RawTimeData);
                    m_TimeInfo = std::localtime(&m_RawTimeData);
                    m_TimeString = std::asctime(m_TimeInfo);
                }
            #endif
    };
}

#endif
////////////////////////////////////////////////////////////
/// \class Phox::Date
///
/// This is a simple class that will return the current date and time.
///
/// Usage example:
/// \code
/// std::cout << "Time is: " << Phox::Date.Hour() << ":" << Phox::Date.Minute() << ":" << Phox::Date.Second();
/// //This could output: 11:14:52, being almost 11:15 am.
/// \endcode
///
////////////////////////////////////////////////////////////
