#ifndef MONITOR_H
#define MONITOR_H

#include <deque>
#include <list>
#include <queue>
#include <string>



namespace de
{
    namespace graphics
    {
        struct frameDetails;
    }
    namespace sys
    {
        void getFrameDetails( graphics::frameDetails _frameDetails );
        void getFrameRate( const float &_delta );

        const int& FrameRate();
        void Resolution( int &_screenWidth, int &_screenHeight );

        std::string getFrameRate();
        std::string getResolution();
    }
}



#endif //MONITOR_H
