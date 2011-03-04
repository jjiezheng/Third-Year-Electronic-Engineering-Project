#ifndef SCROLLING_BACKGROUND_H
#define SCROLLING_BACKGROUND_H

#include "CoreEngine.h"



class Scrollingbackground
{
    public:
        Scrollingbackground();
        ~Scrollingbackground();

        void reLoadTextures();
        void logic( Uint32 );
        void render( de::enums::fboTypes _fbo );
    private:
        de::classes::Sprite Clouds;

        bool toTransparent0, toOpaque0;
        bool toTransparent1, toOpaque1;

        float Alpha0, Alpha1;
        de::classes::Frect texRect0, texRect1;
};
#endif //SCROLLING_BACKGROUND_H

