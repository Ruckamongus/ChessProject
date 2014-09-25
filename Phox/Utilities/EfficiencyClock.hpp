#ifndef __PhoxEngineEfficiencyClock__
#define __PhoxEngineEfficiencyClock__
//#include <ctime>
#include <ratio>
#include <chrono>

namespace Phox
{
    class cEfficiencyClock
    {
        public:
            std::chrono::hours Hours;
            std::chrono::minutes Minutes;
            std::chrono::seconds Seconds;
            std::chrono::milliseconds Milliseconds;
            std::chrono::microseconds Microseconds;
            std::chrono::nanoseconds Nanoseconds;

            #ifdef PHOX_ENABLE_INLINES
                void start() {m_Time1 = std::chrono::high_resolution_clock::now();}
                void stop()  {m_Time2 = std::chrono::high_resolution_clock::now();}
            #else
                void start();
                void stop();
            #endif


            template <class T>
            double get(T TimeType) const
            {return (std::chrono::duration_cast<decltype(TimeType)> (m_Time2 - m_Time1)).count();}

        private:
            std::chrono::high_resolution_clock::time_point m_Time1;
            std::chrono::high_resolution_clock::time_point m_Time2;
    };
}
#endif
