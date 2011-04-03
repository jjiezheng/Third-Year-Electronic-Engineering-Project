#include "Timer.h"
#include "Monitor.h"


#include <SDL/SDL.h>
#include <string>


namespace de
{
    namespace time
    {
        //If a varible is passed in for the framerate its stored here, otherwise it defaults to 60fde
        MainTimer::MainTimer( const float &_framesPerSecond ) :
            startTime(0), pausedTime(0), currentTime(0), framesPerSecond(_framesPerSecond), paused(false), started(false)
        {
            SDL_Init( SDL_INIT_TIMER );

            timer.start();
        }

        void MainTimer::start()
        {
            started = true;
            paused = false;
            startTime = SDL_GetTicks();
        }

        void MainTimer::stop()
        {
            started = false;
            paused = false;
            currentTime = 0;
        }

        void MainTimer::pause()
        {
            if( started && !paused )
            {
                paused = true;
                pausedTime = SDL_GetTicks() - startTime;
            }
        }

        void MainTimer::unpause()
        {

            //If the timer is paused
            if( paused == true )
            {
                //Unpause the timer
                paused = false;

                //Reset the starting ticks
                startTime = SDL_GetTicks() - pausedTime;

                //Reset the paused ticks
                pausedTime = 0;
            }
        }

        unsigned int& MainTimer::getTime()
        {
            if( started == true )
            {
                if( paused == true )
                {
                    return pausedTime;
                }
                else
                {
                    currentTime = SDL_GetTicks() - startTime;
                    return currentTime;
                }
            }
            return currentTime;
        }

        bool MainTimer::is_started()
        {
            return started;
        }

        bool MainTimer::is_paused()
        {
            return paused;
        }

        void MainTimer::frame_cap()
        {
            float timePerFrame( 1000.0f / framesPerSecond );
            float sinceLastFrame = timer.stopstartAndSample() / 1000.0f;
            float delay = timePerFrame - sinceLastFrame;

            if( delay > 0.0 )
            {
                SDL_Delay( (Uint32)delay );
            }

            timer.start();
        }
    }
}


