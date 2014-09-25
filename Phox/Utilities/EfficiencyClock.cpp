#include <Phox/Utilities/EfficiencyClock.hpp>

namespace Phox
{
    #ifndef PHOX_ENABLE_INLINES
        void cEfficiencyClock::start()
        {
            m_Time1 = std::chrono::high_resolution_clock::now();
        }

        void cEfficiencyClock::stop()
        {
            m_Time2 = std::chrono::high_resolution_clock::now();
        }
    #endif
}
