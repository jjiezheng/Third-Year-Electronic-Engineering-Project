#include "Monitor.h"

#include "Functions.h"
#include "Graphics.h"






namespace de
{
    namespace sys
    {

        float currentFrameRate, Timer;
        int frame;
        int screenWidth, screenHeight;



        void getFrameDetails( graphics::frameDetails _frameDetails )
        {
            screenWidth = _frameDetails.screenWidth;
            screenHeight = _frameDetails.screenHeight;
        }

        void getTime( const int &_delta )
        {
            frame++;
            Timer += _delta;

            if ( Timer > 1000 )
            {
                currentFrameRate =  frame / ( Timer / 1000.f );
                Timer = frame = 0;
            }
        }

        const float& FrameRate()
        {
            return currentFrameRate;
        }

        void Resolution( int &_screenWidth, int &_screenHeight )
        {
            _screenWidth = screenWidth;
            _screenHeight = screenHeight;
        }




        std::string getFrameRate()
        {
            return ( "FPS:" + de::misc::toString( FrameRate() ) );
        }
        std::string getResolution()
        {
            return ( "Resolution:" + de::misc::toString(screenWidth) + "X" + de::misc::toString(screenHeight) );
        }

    }
}
