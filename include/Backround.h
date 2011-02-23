#ifndef BACKROUND_H
#define BACKROUND_H

#include "CoreEngine.h"
#include "Scrollingbackground.h"




class Backround
{
    public:
        Backround();
        void reLoadTextures();
        void logic( Uint32 );
        void render();

    private:
        Scrollingbackground background;
        de::classes::Poly leftBorder, rightBorder;
};
#endif //BACKROUND_H
