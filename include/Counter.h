#ifndef COUNTER_H
#define COUNTER_H

#include <SDL/SDL.h>

namespace de
{
    namespace time
    {
        //! A counter class. Used anywhere where timing is important.
        /*! Use with just count() or with count( Uint32 _deltaTicks ) and checkReset().
        */
        class Counter
        {
        public:
            //! Basic Construtor. You will need to run setUp afterwards.
            Counter();
            //! Normal constructor.
            Counter( int _countReset, int _countDelay );
            //! To be used after the Basic Constructor.
            void setUp( int _countReset, int _countDelay );

            //! Set the gap in milliseconds between each count.
            void countDelayIs( int _countDelay );
            //! Set the Maximum count before reseting to 0.
            void countResetIs( int _countReset );
            //! Get the current count.
            int getCurrentCount();
            //! Get the getOverFlow.
            int getOverFlow();

            //! Count up by 1. If the counter needs to be reset then a true is returned.
            bool count();
            //! Count according to the countDelay set by countDelayIs( int _countDelay ). If the counter is incremented then a true is returned.
            bool count( const Uint32 & _deltaTicks );
            //! Decrementes _deltaTicks by countDelay and returns _deltaTicks.
            int countDown( int &_deltaTicks );

            //! Checks to see if the counter needs to be reset. To be used with count( Uint32 _deltaTicks ).
            bool checkReset();

        //private:
            int countReset, currentCount, countDelay, deltaCount, overFlow;
        };
    }
}

#endif // COUNTER_H
