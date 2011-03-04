#include "MainMenu.h"

#include "Entities.h"
#include "pixelstorm.h"

#include "Menu.h"
#include "VerticalShooter.h"
#include "LevelEditor.h"

using namespace de::classes;
using namespace de::graphics;
using namespace de::core;
using namespace de::enums;
using namespace de::io;


//Constructor
MainMenu::MainMenu() :
    firstTime(true),
    showMenu(true),
    setSound(false),
    setGraphics(false),
    setJoystick(false),
    setKeyboard(false)
{
    changeState = false;

    /*int i = resourceManager->getMusic( "Neptune" );
    Sound::playMusic( i );*/
}

MainMenu::~MainMenu()
{}

inline void MainMenu::setStaticElements()
{

    backround.setTexture( "Main_Menu_Backround" );
    logo.setTexture( "Logo", "Alpha" );

    Frect rect;
    rect.x = 1280/2;
    rect.y = 800/2;
    rect.w = 1280;
    rect.h = 800;

    backround.coords.Setup( rect, 0 );
    backround.texCoords.Setup( 0.625, 0.78125 );

    backround.priority = 2;


    rect.x = 1280/2;
    rect.y = 90;
    rect.w = 760;
    rect.h = 56;

    logo.coords.Setup( rect , 0 );
    logo.texCoords.Setup( 0.7421875 , 0.875 );
    logo.priority = 3;

}

bool MainMenu::handleEvents( const SDL_Event &_event )
{
    if( _event.type == SDL_USEREVENT )
    {
        if( _event.user.code == events::OPENGL_RELOAD )
        {
            reLoadTextures();
        }
    }

    if( showMenu ) menu.handleEvent( _event );

    if( setKeyboard ) Keyboard.handleEvent( _event );
    if( setJoystick ) Joystick.handleEvent( _event );


    if ( _event.type == SDL_KEYDOWN )
    {
        if ( _event.key.keysym.sym )
        {
            if ( _event.key.keysym.sym == SDLK_p )
                de::Engine::Audio().pauseMusic();
        }
    }

    return true;
}

bool MainMenu::logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options )
{
    if(firstTime)
    {
        firstTime = !firstTime;
        setStaticElements();
    }
    if( setKeyboard ) setKeyboard = Keyboard.logic();
    if( setJoystick ) setJoystick = Joystick.logic();

    if( showMenu && !setKeyboard && !setJoystick )
    {
        switch( menu.logic( _deltaTicks ) )
        {
            case NEW_GAME:
                _nextState = new VerticalShooter();
                break;

            case LEVEL_EDITOR:
                _nextState = new LevelEditor();
                break;

            case SET_KEYBOARD:
                setKeyboard = true;
                break;

            case SET_JOYSTICK:
                setJoystick = true;
                break;

            case SET_GRAPHICS:
                setGraphics = true;
                break;

            case SET_SOUND:
                setSound = true;
                break;

            case EXIT:
                _options = de::state::EXIT;
                break;
        }
    }

    return false;
}

void MainMenu::reLoadTextures()
{
    backround.reload();
    logo.reload();
    menu.reLoadTextures();
}

void MainMenu::render()
{
    backround.render( FBO_UI );
    logo.render( FBO_UI );

    if( showMenu )      menu.render();
    if( setKeyboard )   Keyboard.render();
    if( setJoystick )   Joystick.render();

}
