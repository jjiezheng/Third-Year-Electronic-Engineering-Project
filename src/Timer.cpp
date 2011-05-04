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
            startTime(0), 
			pausedTime(0), 
			currentTime(0), 
			framesPerSecond(_framesPerSecond), 
			timePerFrame(1000.0f / _framesPerSecond), 
			deltaFrame(0), 
			sinceLastFrame(0), 
			paused(false), 
			started(false)
        {
            SDL_Init( SDL_INIT_TIMER );

            timer.start();
        }

        void MainTimer::start()
        {
            started = true;
            startTime = SDL_GetTicks();
        }

        void MainTimer::stop()
        {
            /*started = false;
            paused = false;
            currentTime = 0;*/
        }

        void MainTimer::pause()
        {/*
            if( started && !paused )
            {
                paused = true;
                pausedTime = SDL_GetTicks() - startTime;
            }*/
        }

        void MainTimer::unpause()
        {
			/*
            //If the timer is paused
            if( paused == true )
            {
                //Unpause the timer
                paused = false;

                //Reset the starting ticks
                startTime = SDL_GetTicks() - pausedTime;

                //Reset the paused ticks
                pausedTime = 0;
            }*/
        }

        float MainTimer::getTime()
        {
            return deltaFrame;
        }

		float MainTimer::getFrameRate()
		{
			return (1000.0f/sinceLastFrame);
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
			timer.stop();
            sinceLastFrame = timer.sample()/1000.0f;
			if( sinceLastFrame < timePerFrame )
			{

				//SDL_Delay( delay );
				Sleep( timePerFrame - sinceLastFrame );
				deltaFrame = timePerFrame;
			}
			else
			{
				deltaFrame = sinceLastFrame;
			}
            timer.start();
        }
    }
}


