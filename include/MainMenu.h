#ifndef MAINMENU_H
#define MAINMENU_H

#include "State.h"

class MainMenu : public de::state::State
{
    public:
        MainMenu();
        ~MainMenu();

        bool handleEvents( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options );
        void render();


    private:
        void reLoadTextures();

        bool firstTime, showMenu, setSound, setGraphics, setJoystick, setKeyboard;

        de::classes::Sprite backround, logo;
        Menu menu;
        SetKeyboard Keyboard;
        SetJoystick Joystick;

        void setStaticElements();
        std::vector<de::classes::Sprite> renderStaticElements();
};

#endif // MAINMENU_H
