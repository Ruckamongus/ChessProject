#include <Chess\John\EfficiencyClockCBinding.hpp>
#include <Phox\Utilities\EfficiencyClock.hpp>
namespace
{
    Phox::cEfficiencyClock Clock;
}

void clockStart()
{
    Clock.start();
}

void clockStop()
{
    //Clock.stop();
}
double clockGet(timeBase t)
{
    switch (t)
    {
        case sec:
            return Clock.get(Clock.Seconds);
        case milli:
            return Clock.get(Clock.Milliseconds);
        case micro:
            return Clock.get(Clock.Microseconds);
        case nano:
            return Clock.get(Clock.Nanoseconds);
        default: return -1.d;
    }
}
