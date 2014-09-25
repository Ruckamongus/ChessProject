#ifndef __EfficiencyClockC__
#define __EfficiencyClockC__
#include <Phox/Utilities/EfficiencyClock.hpp>

extern "C"
{
    enum timeBase
    {
        sec,
        milli,
        micro,
        nano
    };

    void   clockStart();
    void   clockStop();
    double clockGet(timeBase t);
}
#endif
