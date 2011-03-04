#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

#include <SDL/SDL.h>
#include <string>

namespace de
{
    namespace time
    {
        class TimeCounter
        {
            public:
                TimeCounter();
                ~TimeCounter();

                void reset();
                void addMilliSeconds( const Uint32 &_deltaTicks );
                const std::string &getTimeString();


                const unsigned int getMilliSeconds();
                const unsigned int &getSeconds();

            private:
                unsigned int totalMilliSeconds, milliSeconds, Seconds, Minutes, Hours, Days, Years;
                std::string timeString;
        };
    }
}


#endif // TIMECOUNTER_H
