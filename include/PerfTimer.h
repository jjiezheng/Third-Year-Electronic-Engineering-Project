#ifndef PERFTIMER_H
#define PERFTIMER_H

#include <platformstl/performance/performance_counter.hpp>



namespace de
{
    namespace time
    {

        class PerfTimer
        {
            public:
                PerfTimer() {}
                virtual ~PerfTimer() {}

                void start();
                void stop();
                int sample();
                int sampleMilli();
                int stopstartAndSample();
            private:
                platformstl::performance_counter    PCounter;
        };
    }
}

#endif // PERFTIMER_H
