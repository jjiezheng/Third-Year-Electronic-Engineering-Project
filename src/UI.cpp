#include "UI.h"

using namespace de::classes;
using namespace de::graphics;
using namespace de::theme;
using namespace de::misc;
using namespace de::enums;

//Handles ( guess ) the UI,
UI::UI( Stats* _stats ) :
    innerMenuBox( 200, 200, true ),
    outerMenuBox( 210, 210, true )
{
    stats = _stats;

    innerMenuBox.transform( -innerMenuBox.centre + Vector(640, 370) );
    outerMenuBox.transform( -outerMenuBox.centre + Vector(640, 370) );

    PauseMenu.setGade( 0, 40, "vertical" );
    PauseMenu.add( Vector(640, 350), RESUME, "Resume", DefaultTextColour );
    PauseMenu.add( QUIT, "Quit" );

    ContinueMenu.setGade( 100, 0, "horizontal" );
    ContinueMenu.add( Vector(485, 450), RESUME, "Yes", DefaultTextColour );
    ContinueMenu.add( QUIT, "No" );


    localPause = false;
    mainCurser = -1;

    credit.set( "CREDITS:",     "Unsteady", Vector(50, 650), 10, ALIGN_CENTRE );
    score.set( "SCORE:",        "Unsteady", Vector(700, 770), 10, ALIGN_CENTRE );
    life.set( "LIVES:",         "Unsteady", Vector(1140, 781), 10, ALIGN_CENTRE );
    timePassed.set( "TIME:0.00","Unsteady", Vector(45, 781), 10, ALIGN_RIGHT );



    Continue.set( "Continue?", "GeoSansLight",  Vector(475, 250) );

    life.setColour( DefaultTextColour );
    credit.setColour( DefaultTextColour );
    timePassed.setColour( DefaultTextColour );

    score.setColour( Colour( DefaultBoxColour.r,DefaultBoxColour.g,DefaultBoxColour.b, 0.5 ) );
}


void UI::handleEvent( const SDL_Event &_event, bool &_pause )
{
    if( stats->lives > 0 )
    {
        PauseMenu.handleEvent( _event );

        if ( _event.type == SDL_KEYDOWN )
        {
            if ( _event.key.keysym.sym == SDLK_ESCAPE )
                _pause = !_pause;
        }
    }
    else
        ContinueMenu.handleEvent( _event );

    localPause = _pause;
}

bool UI::logic( const std::string &_time, bool &_pause )
{
    /*static int lifeNumber(0), creditNumber(0), scoreNumber(1);

    if( lifeNumber != stats->lives )
    {
        lifeNumber = stats->lives;

    }
*/
    life = "LIVES:" + toString( stats->lives );
    credit = "CREDITS:"  + toString( stats->credits );
    score = "SCORE:" + getZeros( stats->score ) + toString( stats->score );
    timePassed = "TIME:" + _time;



    if( _pause )
    {
        mainCurser = PauseMenu.logic( 1 );
        switch( mainCurser )
        {
            case RESUME:
            _pause = !_pause;
            break;

            case QUIT:
            return true;
        }
    }


    if( stats->lives < 0 )
    {
        _pause = true;
        mainCurser = ContinueMenu.logic( 1 );
        switch( mainCurser )
        {
            case RESUME:
            if( stats->credits > 0 )
            {
                stats->credits--;
                stats->lives = 4;
                _pause = false;
            }
            break;

            case QUIT:
            return true;
        }
    }

    return false;
}

std::string UI::getZeros( const int &_number )
{
    if( _number < 10 )
        return "00000";

    else if( _number < 100 )
        return "0000";

    else if( _number < 1000 )
        return "000";

    else if( _number < 10000 )
        return "00";

    else if( _number < 100000 )
        return "0";

    else return "";
}

void UI::reLoadTextures()
{
    PauseMenu.reLoadTextures();
    ContinueMenu.reLoadTextures();


    life.reload();
    credit.reload();
    score.reload();
    Continue.reload();
}


void UI::render()
{
    life.render( FBO_UI );
    //credit.render( FBO_UI );
    score.render( FBO_UI );
    timePassed.render( FBO_UI );


    if( localPause && stats->lives > 0 )
    {
        /*de::Engine::Graphics().add( outerMenuBox, DefaultBoxColour, 9, FBO_UI );
        de::Engine::Graphics().add( innerMenuBox, DefaultSelectedBoxColour, 10, FBO_UI );
        PauseMenu.render();*/
    }


    if( stats->lives < 0 )
    {/*
        ContinueMenu.render();
        Continue.render( FBO_UI );*/
    }
}
