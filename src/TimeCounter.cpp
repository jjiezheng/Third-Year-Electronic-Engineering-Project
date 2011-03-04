#include "TimeCounter.h"
#include "TimeCounter.h"

#include "Functions.h"

using namespace de::misc;

namespace de
{
    namespace time
    {
        TimeCounter::TimeCounter()
            :milliSeconds(0), Seconds(0), Minutes(0), Hours(0), Days(0), Years(0)
        {}

        TimeCounter::~TimeCounter() {}
        //WTF dan, use %
        void TimeCounter::addMilliSeconds( const Uint32 &_deltaTicks )
        {
            milliSeconds += _deltaTicks;
            if( milliSeconds > 1000 )
            {
                while( milliSeconds > 1000 )
                {
                    milliSeconds -= 1000;
                    ++Seconds;
                }

                if( Seconds >= 60 )
                {
                    while( Seconds >= 60 )
                    {
                        Seconds -= 60;
                        ++Minutes;
                    }

                    if( Minutes >= 60 )
                    {
                        while( Seconds >= 60 )
                        {
                            Minutes -= 60;
                            ++Hours;
                        }

                        if( Hours >= 24 )
                        {
                            while( Hours >= 24 )
                            {
                                Hours -= 24;
                                ++Days;
                            }

                            if( Days >= 365 )
                            {
                                while( Days >= 365 )
                                {
                                    Days -= 365;
                                    ++Years;
                                }
                            }

                        }
                    }
                }
            }
        }

        const std::string &TimeCounter::getTimeString()
        {
            unsigned int tempMilli( milliSeconds/10 );
            timeString = toString(Minutes) + ":" + toString(Seconds) + ":" + toString(tempMilli);
            return timeString;
        }


        const unsigned int &TimeCounter::getSeconds()
        {
            return Seconds;
        }

        const unsigned int TimeCounter::getMilliSeconds()
        {
            return milliSeconds + Seconds*1000 + Minutes*60*1000;
        }


        void TimeCounter::reset()
        {
            milliSeconds = Seconds = Minutes = Hours = Days = Years =0;
        }
    }
}
