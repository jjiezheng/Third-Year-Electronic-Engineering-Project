#ifndef TITLE_H
#define TITLE_H

#include "State.h"
#include "Object3D.h"
#include "SpriteInstance.h"
#include "SpriteObject.h"


class Title : public de::state::State
{
    public:
        Title();
        ~Title();

        bool handleEvents( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options );

        void render();

    private:
        void reLoadTextures();

        int TimeCounter;
        bool doAnimation, usingOpenGL;

        de::classes::Animation animation;

        de::classes::Sprite logo;
        de::classes::SpriteString Presents, Name;
};

#endif // TITLE_H
