#ifndef CONTROLMENU_H
#define CONTROLMENU_H

#include "CoreEngine.h"
#include "Coords.h"

#include <string>
#include <vector>
#include <SDL/SDL.h>



class SetKeyboard
{
    public:
        SetKeyboard();

        void handleEvent( const SDL_Event &event );
        bool logic();
        void reLoadTextures();
        void render();

    private:
        bool finished, keyPressed, first;
        SDLKey currentKey;
        de::enums::enumSettings currentState;

        de::classes::SpriteString currentText;
        de::classes::Poly box;

        void updateText();
};


class SetJoystick
{
    public:
        SetJoystick();

        void handleEvent( const SDL_Event &event );
        bool logic();
        void reLoadTextures();
        void render();

    private:
        bool finished, keyPressed, gotJoystick, first;
        Uint8 currentKey;
        de::enums::enumSettings currentState;
        SDL_Joystick* stick;

        de::classes::SpriteString currentText, pickJoy;
        de::classes::Poly box;

        void updateText();
};


#endif //CONTROLMENU_H
