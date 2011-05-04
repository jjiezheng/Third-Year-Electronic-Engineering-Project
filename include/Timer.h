#ifndef MAINTIMER_H
#define MAINTIMER_H

#include <SDL/SDL.h>
#include "PerfTimer.h"
#include "Log.h"


namespace de
{
    namespace time
    {
        //! SDL based timer used to regulate the framerate and control ingame logic speed.
        class MainTimer
        {
            public:
                    //! Constructor. Default framerate is 60Fde.
                    MainTimer( const float &_framesPerSecond = 60.0 );

                    //! Start the timer
                    void start();
                    //! Stop the timer
                    void stop();
                    //! Pause the timer
                    void pause();
                    //! Unpause the timer
                    void unpause();
                    //! Used at the end of a frame after Graphics::render(). Waits till its time for the next frame.
                    void frame_cap();

                    //! Returns how much time has passed since the last frame.
                    float getTime();
					float getFrameRate();

                    //! Returns true if the Timer is started
                    bool is_started();
                    //! Returns true if the Timer is paused
                    bool is_paused();


                private:
                    PerfTimer timer;
                    unsigned int startTime;
                    unsigned int pausedTime;
                    unsigned int currentTime;
                    float framesPerSecond, sinceLastFrame, timePerFrame, deltaFrame;
                    bool paused;
                    bool started;
        };
    }
}
#endif //MAINTIMER_H
