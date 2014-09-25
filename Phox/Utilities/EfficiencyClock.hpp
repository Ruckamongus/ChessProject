#ifndef __PhoxEngineEfficiencyClock__
#define __PhoxEngineEfficiencyClock__
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

            inline void start()
            {
                m_Time1 = std::chrono::high_resolution_clock::now();
            }

            template <class T>
            double get(T TimeType) const
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                return (std::chrono::duration_cast<decltype(TimeType)> (endTime - m_Time1)).count();
            }

        private:
            std::chrono::high_resolution_clock::time_point m_Time1;
    };
}
#endif
