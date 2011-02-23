#include "ControlMenu.h"

using namespace de::classes;
using namespace de::graphics;
using namespace de::enums;
using namespace de::theme;


SetKeyboard::SetKeyboard()
{
    currentState = C_KEYBOARD_UP;
    finished = keyPressed = first = false;

    box.Setup( 600, 50, true );
    box.transform( Vector(640,420), 0 );
}

void SetKeyboard::handleEvent( const SDL_Event &event )
{
    if ( event.type == SDL_KEYDOWN )
    {
        if ( event.key.keysym.sym )
        {
            if( event.key.keysym.sym == SDLK_ESCAPE )
            {
                finished = true;
                return;
            }
            currentKey = event.key.keysym.sym;
            keyPressed = true;
            if( currentState == C_KEYBOARD_SPECIAL )
                finished = true;
        }
    }
}

bool SetKeyboard::logic()
{
    if( !first )
    {
        first = !first;
        currentText.set( "Please enter a key for up", "GeoSansLight", Vector( 640, 425 ), 13, ALIGN_CENTRE );
        currentText.setColour(DefaultTextColour);
    }

    if( currentState != S_NOTHING && keyPressed  )
    {
        //Settings::changeData( currentState, currentKey );
        updateText();
        keyPressed = false;
    }

    if( finished )
    {
        currentState = C_KEYBOARD_UP;
        currentText = "Please enter a key for up";
        finished = keyPressed = false;
        return false;
    }

    return true;
}

void SetKeyboard::render()
{
    de::Engine::Graphics().add( box, DefaultSelectedBoxColour, 5, FBO_UI );
    currentText.render(FBO_UI);
}

void SetKeyboard::updateText()
{
    if( currentState == C_KEYBOARD_UP )
    {
        currentText = "Please enter a key for down";
        currentState = C_KEYBOARD_DOWN;
        return;
    }

    if( currentState == C_KEYBOARD_DOWN )
    {
        currentText = "Please enter a key for left";
        currentState = C_KEYBOARD_LEFT;
        return;
    }

    if( currentState == C_KEYBOARD_LEFT )
    {
        currentText = "Please enter a key for right";
        currentState = C_KEYBOARD_RIGHT;
        return;
    }

    if( currentState == C_KEYBOARD_RIGHT )
    {
        currentText = "Please enter a key for fire";
        currentState = C_KEYBOARD_FIRE;
        return;
    }

    if( currentState == C_KEYBOARD_FIRE )
    {
        currentText = "Please enter a key for fire 2";
        currentState = C_KEYBOARD_FIRE2;
        return;
    }

    if( currentState == C_KEYBOARD_FIRE2 )
    {
        currentText = "Please enter a key for special";
        currentState = C_KEYBOARD_SPECIAL;
        return;
    }

    if( currentState == S_NOTHING )
    {
        currentText = "NONE";
        return;
    }
}



SetJoystick::SetJoystick()
{
    currentState = C_JOYSTICK_NUMBER;
    finished = keyPressed = gotJoystick = first = false;


    box.Setup( 600, 50, true );
    box.transform( Vector(640,420), 0 );
}

void SetJoystick::handleEvent( const SDL_Event &event )
{
    if ( event.type == SDL_KEYDOWN )
    {
        if ( event.key.keysym.sym )
        {
            if( event.key.keysym.sym == SDLK_ESCAPE )
            {
                finished = true;
                return;
            }
        }
    }

    if( !gotJoystick )
    {

        if( event.type == SDL_JOYBUTTONDOWN )
        {
            if( event.jbutton.state == SDL_PRESSED )
            {
                currentKey = (int)event.jbutton.which;
                keyPressed = true;
            }
        }
    }
    else if ( event.type == SDL_JOYBUTTONDOWN )
    {
        /*if( event.jbutton.which == Settings::get( "JoystickNumber" ) )
        {*/
            if( ( event.type == SDL_JOYBUTTONDOWN ) )
            {
                currentKey = (int)event.jbutton.button;
                keyPressed = true;
                if( currentState == C_JOYSTICK_SPECIAL )
                    finished = true;
            }
        /*}*/
    }
}

bool SetJoystick::logic()
{
    if( !first )
    {
        first = !first;
        currentText.set( "Please enter a key for fire", "GeoSansLight", Vector( 640, 425 ), 13, ALIGN_CENTRE );
        pickJoy.set( "Press any button on the joystick you want to use", "GeoSansLight", Vector( 640, 425 ), 13, ALIGN_CENTRE );

        currentText.setColour(DefaultTextColour);
        pickJoy.setColour(DefaultTextColour);
    }

    if( currentState != S_NOTHING && keyPressed  )
    {
        //Settings::changeData( currentState, currentKey );
        updateText();
        keyPressed = false;
        gotJoystick = true;
    }

    if( finished )
    {
        currentState = C_JOYSTICK_NUMBER;
        finished = keyPressed = gotJoystick = false;
        return false;
    }

    return true;
}

void SetJoystick::render()
{
    de::Engine::Graphics().add( box, DefaultSelectedBoxColour, 12, FBO_UI );

    if( !gotJoystick )
        pickJoy.render(FBO_UI);
    else
        currentText.render(FBO_UI);
}


void SetJoystick::updateText()
{
    if( currentState == C_JOYSTICK_NUMBER )
    {
        currentText = "Please enter a key for fire";
        currentState = C_JOYSTICK_FIRE;
        return;
    }

    else if( currentState == C_JOYSTICK_FIRE )
    {
        currentText = "Please enter a key for fire 2";
        currentState = C_JOYSTICK_FIRE2;
        return;
    }

    else if( currentState == C_JOYSTICK_FIRE2 )
    {
        currentText = "Please enter a key for special";
        currentState = C_JOYSTICK_SPECIAL;
        return;
    }

    else if( currentState == C_JOYSTICK_SPECIAL )
    {
        currentText = "Please enter a key for special";
        currentState = S_NOTHING;
        return;
    }
}


