#ifndef FRAMEDETAILS_H
#define FRAMEDETAILS_H

#include "CoreGraphics.h"



namespace de
{
    namespace graphics
    {
        struct frameDetails
        {
            frameDetails( int _spriteNumber, int _batchNumber, int _screenWidth, int _screenHeight )
                :spriteNumber( _spriteNumber ), batchNumber( _batchNumber ), screenWidth( _screenWidth ), screenHeight( _screenHeight ) {}

            frameDetails()
                :spriteNumber( 0 ), batchNumber( 0 ), screenWidth( 0 ), screenHeight( 0 ) {}

            int spriteNumber;
            int batchNumber;
            float screenWidth, screenHeight;
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
