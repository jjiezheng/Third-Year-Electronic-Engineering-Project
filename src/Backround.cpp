#include "backround.h"
#include <iostream>

using namespace de::classes;
using namespace de::graphics;
using namespace de::enums;
using namespace de::theme;


Backround::Backround()
{

    leftBorder.Setup( 400, 800, false );
    rightBorder.Setup( 280, 800, false );
    rightBorder.transform( Vector( 1000,0 ) );
}

void Backround::reLoadTextures()
{
    background.reLoadTextures();
}

void Backround::logic( Uint32 deltaTicks )
{
    background.logic( deltaTicks );
}




void Backround::render()
{
    background.render( FBO_GAME );

    de::Engine::Graphics().add( leftBorder,  DefaultSelectedBoxColour, 1, FBO_UI );
    de::Engine::Graphics().add( rightBorder, DefaultSelectedBoxColour, 1, FBO_UI );
}
