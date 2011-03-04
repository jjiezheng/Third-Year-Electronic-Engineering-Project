#include "Counter.h"

#include "Monitor.h"


#include <SDL\SDL.h>
#include <string>


namespace de
{
    namespace time
    {
        Counter::Counter( int _countReset, int _countDelay )
        {
            countReset = _countReset;
            countDelay = _countDelay;

            currentCount = deltaCount = 0;
        }

        Counter::Counter()
        {
            countReset = countDelay = currentCount = deltaCount = overFlow = 0;
        }


        void Counter::setUp( int _countReset, int _countDelay )
        {
            countReset = _countReset;
            countDelay = _countDelay;
        }

        void Counter::countResetIs( int _countReset )
        {
            countReset = _countReset;
        }

        void Counter::countDelayIs( int _countDelay )
        {
            countDelay = _countDelay;
        }


        bool Counter::count( const Uint32 &_deltaTicks )
        {
            deltaCount += _deltaTicks;

            if( deltaCount >= countDelay  )
            {
                deltaCount = 0;
                ++currentCount;
                overFlow = deltaCount - countDelay;

                return true;
            }
            return false;
        }

        int Counter::getOverFlow()
        {
            return overFlow;
        }

        int Counter::countDown( int &_deltaTicks )
        {
            if( _deltaTicks >= countDelay  )
            {
                _deltaTicks += deltaCount;
                deltaCount = 0;

                _deltaTicks -= countDelay;
                ++currentCount;

                return _deltaTicks;
            }
            else
            {
                deltaCount += _deltaTicks;
            }
            return 0;
        }

        bool Counter::count()
        {
            ++currentCount;

            if( currentCount >= countReset )
            {
                currentCount = 0;
                return true;
            }
            return false;
        }

        int Counter::getCurrentCount()
        {
            return currentCount;
        }

        bool Counter::checkReset()
        {
            if( currentCount >= countReset )
            {
                currentCount = 0;
                return true;
            }

            return false;
        }
    }
}
