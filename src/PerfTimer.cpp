#include "PerfTimer.h"



namespace de
{
    namespace time
    {
        void PerfTimer::start()
        {
            PCounter.start();
        }


        void PerfTimer::stop()
        {
            PCounter.stop();

        }

        int PerfTimer::sample()
        {
            return PCounter.get_microseconds();
        }
        int PerfTimer::sampleMilli()
        {
            return PCounter.get_milliseconds();
        }

        int PerfTimer::stopstartAndSample()
        {
            PCounter.stop();
            int time = PCounter.get_microseconds();
            PCounter.start();

            return time;
        }
    }
}
