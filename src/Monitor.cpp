#include "Monitor.h"

#include "Functions.h"
#include "Graphics.h"






namespace de
{
    namespace sys
    {

        int currentFrameRate;
        int screenWidth, screenHeight;



        void getFrameDetails( graphics::frameDetails _frameDetails )
        {
            screenWidth = _frameDetails.screenWidth;
            screenHeight = _frameDetails.screenHeight;
        }

        void getFrameRate( const float &_delta )
        {
            currentFrameRate = (int)(_delta+0.5f);
        }

        const int& FrameRate()
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
