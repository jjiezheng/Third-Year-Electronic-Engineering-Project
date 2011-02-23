#ifndef UI_H
#define UI_H

#include "Menu.h"
#include "Player.h"
#include "CoreEngine.h"



enum IngameMenu
{
    RESUME,
    QUIT
};




class UI
{
private:
    bool localPause;
    int mainCurser;

    Stats* stats;

    MenuList PauseMenu, ContinueMenu;
    de::classes::SpriteString life, credit, score, Continue, timePassed;
    de::classes::Poly innerMenuBox, outerMenuBox;

    std::string getZeros( const int &_number );
public:
    UI( Stats* );

    bool logic( const std::string &_time, bool &_pause );

    void handleEvent( const SDL_Event &_event, bool &_pause );
    void reLoadTextures();


    void render();
};

#endif //UI_H
