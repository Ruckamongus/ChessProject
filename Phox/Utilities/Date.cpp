#include <Phox/Utilities/Date.hpp>
namespace
{
    bool stringOnlyContains(const std::string& String, const std::string& StringContains)
    {
        for (unsigned int i = 0; i < String.size(); i++)
        {
            bool Good = 0;
            for (unsigned int j = 0; j < StringContains.size(); j++)
            {
                if (String[i] == StringContains[j]) Good = 1;
            }

            if (!Good) return false;
        }

        return true;
    }

    std::string stringGetNthWord(const std::string& String, unsigned int N, const std::string& Delimiters = " ")
    {
        unsigned int CurrentWord = 0;
        std::string CurrentString;

        for (unsigned int i = 0; i < String.size(); i++)
        {
            bool Bad = 0;
            for (unsigned int j = 0; j < Delimiters.size(); j++)
            {
                if (String[i] == Delimiters[j])
                {
                    if (String != "" && !stringOnlyContains(CurrentString, Delimiters))
                    {
                        CurrentWord++;
                        if (CurrentWord == N)
                            return CurrentString;
                    }

                    CurrentString = "";
                    Bad = 1;
                    break;
                }
            }

            if (!Bad) CurrentString += String[i];
        }

        if (!stringOnlyContains(CurrentString, Delimiters))
        {
            if (CurrentWord + 1 == N)
                return CurrentString;
        }

        return "";
    }
}

time_t Phox::Date::m_RawTimeData;
struct std::tm* Phox::Date::m_TimeInfo;
std::string Phox::Date::m_TimeString;

std::string Phox::Date::day()
{
    update();
    return stringGetNthWord(m_TimeString, 3, " :");
}

std::string Phox::Date::hour()
{
    update();
    return stringGetNthWord(m_TimeString, 4, " :");
}

std::string Phox::Date::minute()
{
    update();
    return stringGetNthWord(m_TimeString, 5, " :");
}

std::string Phox::Date::second()
{
    update();
    return stringGetNthWord(m_TimeString, 6, " :");
}

std::string Phox::Date::year()
{
    update();
    return stringGetNthWord(m_TimeString, 7, " :\n");
}

std::string Phox::Date::weekDay()
{
    update();
    std::string Small = stringGetNthWord(m_TimeString, 1, " :");

    if (Small == "Mon") return "Monday";
    if (Small == "Tue") return "Tuesday";
    if (Small == "Wed") return "Wednesday";
    if (Small == "Thu") return "Thursday";
    if (Small == "Fri") return "Friday";
    if (Small == "Sun") return "Sunday";
    if (Small == "Sat") return "Saturday";

    return Small;
}

std::string Phox::Date::month()
{
    update();
    std::string Small = stringGetNthWord(m_TimeString, 2, " :");

    if (Small == "Jan") return "January";
    if (Small == "Feb") return "February";
    if (Small == "Mar") return "March";
    if (Small == "Apr") return "April";
    if (Small == "May") return "May";
    if (Small == "Jun") return "June";
    if (Small == "Jul") return "July";
    if (Small == "Aug") return "August";
    if (Small == "Sep") return "September";
    if (Small == "Oct") return "October";
    if (Small == "Nov") return "November";
    if (Small == "Dec") return "December";

    return Small;
}

unsigned int Phox::Date::dayInteger()
{
    update();
    return m_TimeInfo->tm_mday;
}

unsigned int Phox::Date::monthInteger()
{
    update();
    return m_TimeInfo->tm_mon;
}

unsigned int Phox::Date::yearInteger()
{
    update();
    return m_TimeInfo->tm_year;
}

unsigned int Phox::Date::yearDayInteger()
{
    update();
    return m_TimeInfo->tm_yday;
}

unsigned int Phox::Date::weekDayInteger()
{
    update();
    return m_TimeInfo->tm_wday;
}

unsigned int Phox::Date::hourInteger()
{
    update();
    return m_TimeInfo->tm_hour;
}

unsigned int Phox::Date::minuteInteger()
{
    update();
    return m_TimeInfo->tm_min;
}

unsigned int Phox::Date::secondInteger()
{
    update();
    return m_TimeInfo->tm_sec;
}

bool Phox::Date::daylightSavingsTime()
{
    update();
    return m_TimeInfo->tm_isdst;
}
