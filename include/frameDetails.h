#ifndef FRAMEDETAILS_H
#define FRAMEDETAILS_H

#include "CoreGraphics.h"



namespace de
{
    namespace graphics
    {
        struct frameDetails
        {
            frameDetails( int _screenWidth, int _screenHeight )
                :screenWidth( _screenWidth ), screenHeight( _screenHeight ) {}

            frameDetails()
                :screenWidth( 0 ), screenHeight( 0 ) {}

            int screenWidth, screenHeight;
        };

        struct VideoInfo
        {
            int screenWidth, screenHeight;
            int gameWidth, gameHeight;
            int fullscreenWidth, fullscreenHeight;
            int colorDepth, frameRate;
            int AA, AAFullscreen;
            bool fullScreen, postFX;

            graphics::glVersion::glVersions OpenGL;
            graphics::glslVersion::glslVersions GLSL;
        };
    }
}
#endif // FRAMEDETAILS_H
