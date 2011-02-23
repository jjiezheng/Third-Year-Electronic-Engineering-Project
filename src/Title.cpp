#include "Title.h"


#include "Entities.h"
#include "pixelstorm.h"

#include "Menu.h"
#include "MainMenu.h"


using namespace de::classes;
using namespace de::graphics;
using namespace de::core;
using namespace de::enums;
using namespace de::io;


//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

Title::Title()
{
    changeState = false;

    de::classes::Frect rect, rectTex;
    float width(1), height(1);
    int delay(0), reset(0);

    int screenWidth = 1280;
    int screenHeight = 800;


    rect.x = 0;
    rect.y = 0;
    rect.w = 860;
    rect.h = 138;
    logo.coords.Setup( rect, 0 );

    logo.priority = 10;

    doAnimation = true;
    delay = 100;
    reset = 10;


    logo.setTexture( "Number_12-Colour-Sheet" );
    if( !de::Engine::Resources().getTextureSize( "Number_12-Colour-Sheet", width, height ) )
        de::io::error << "Failure to get texture width and height in Title Constructor\n";



    Vector pos( screenWidth / 2, screenHeight / 2 );
    logo.coords.transform( pos );


    Vector _displacement( 0, rect.h/height );
    animation.setup( (rect.w/width), (rect.h/height) );
    animation.addAnimation( _displacement, delay, reset );


    TimeCounter = 0;

    Presents.set( "PRESENTS", "Unsteady", Vector( 640, 450 ), 11, ALIGN_CENTRE );
    Presents.setColour( Colour( 0, 0, 0, 1 ) );

    Name.set( "DANIEL HARTNETT", "Unsteady", Vector( 640, 375 ), 11, ALIGN_CENTRE );
    Name.setColour( Colour( 0, 0, 0, 1 ) );

}

Title::~Title()
{
}

bool Title::handleEvents( const SDL_Event &_event )
{
    if( _event.type == SDL_USEREVENT )
    {
        if( _event.user.code == events::OPENGL_RELOAD )
        {
            reLoadTextures();
        }
    }


    if ( _event.type == SDL_KEYDOWN )
        changeState = true;

    return true;
}

bool Title::logic( const Uint32 &_deltaTicks, de::state::State* &_nextState, de::state::options &_options )
{
    TimeCounter += _deltaTicks;


    if( changeState )
    {
        //_nextState = new MainMenu();
    }

    if( doAnimation )
        logo.texCoords = animation.loop( _deltaTicks );

    return false;
}

 void Title::reLoadTextures()
{
    logo.reload();
    Presents.reload();
    Name.reload();
}

void Title::render()
{
    //logo.render( FBO_UI );
    Presents.render( FBO_UI );
    Name.render( FBO_UI );
}

