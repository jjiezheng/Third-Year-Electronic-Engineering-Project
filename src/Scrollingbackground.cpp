#include "Scrollingbackground.h"
#include <iostream>

using namespace de::classes;
using namespace de::graphics;
using namespace de::core;
using namespace de::enums;


Scrollingbackground::Scrollingbackground()
{
    //float screenWidth = 1280;
    float screenHeight = 800;

    Clouds.setTexture( "Backround1", "Backround" );
    Clouds.Texture2 = de::Engine::Resources().getTexture( "Backround1" );


    Frect rect;
    rect.x = 275;
    rect.y = screenHeight/2;
    rect.w = 768;
    rect.h = 800;

    texRect0.x = 0.1875;
    texRect0.y = 0.5;
    texRect0.w = 0.375;
    texRect0.h = 1;

    texRect1 = texRect0;
    texRect1.x = 0.6875;


    Clouds.coords.Setup( rect, 0 );
    Clouds.texCoords.Setup( texRect0, 0 );
    Clouds.texCoords2.Setup( texRect1, 0 );


    Alpha0 = 0.1;
    Alpha1 = 0.9;




    toTransparent0 = toTransparent1 = true,
    toOpaque0 = toOpaque1 = false;
}

Scrollingbackground::~Scrollingbackground()
{

}


void Scrollingbackground::reLoadTextures()
{
    Clouds.reload();
    Clouds.Texture2 = de::Engine::Resources().getTexture( "Backround1" );
}

void Scrollingbackground::logic( Uint32 deltaTicks )
{
    Vector velocity( 0, -200 );
    Vector displacement;
    displacement = velocity * ( deltaTicks / 1000.f );


    Clouds.texCoords.transform( Vector( 0,-0.007 ) );
    Clouds.texCoords2.transform( Vector( 0,-0.006 ) );

    if( Alpha0 > 0.0 && toTransparent0 )
    {
        Alpha0 -= 0.01;
        Clouds.texCoords.resize( 0.992 );

        if( Alpha0 < 0.0 )
        {
            toOpaque0 = true;
            toTransparent0 = false;
            Vector centre = Clouds.texCoords.centre;
            if( centre.y < -1 )
                ++centre.y;
            Clouds.texCoords.Setup( texRect0, 0 );
            Clouds.texCoords.transform( centre - Clouds.texCoords.centre );
        }
    }

    if( Alpha0 < 1 && toOpaque0 )
    {
        Alpha0 += 0.01;
        Clouds.texCoords.resize( 0.992 );

        if( Alpha0 > 1.0 )
        {
            toOpaque0 = false;
            toTransparent0 = true;
        }
    }


    if( Alpha1 > 0.0 && toTransparent1 )
    {
        Alpha1 -= 0.01;
        Clouds.texCoords2.resize( 0.992 );

        if( Alpha1 < 0.0 )
        {
            toOpaque1 = true;
            toTransparent1 = false;

            Vector centre = Clouds.texCoords2.centre;
            if( centre.y < -1 )
            {
                ++centre.y;
            }
            Clouds.texCoords2.Setup( texRect1, 0 );
            Clouds.texCoords2.transform( centre - Clouds.texCoords2.centre );
        }
    }


    if( Alpha1 < 1 && toOpaque1 )
    {
        Alpha1 += 0.01;
        Clouds.texCoords2.resize( 0.992 );

        if( Alpha1 > 1.0 )
        {
            toOpaque1 = false;
            toTransparent1 = true;
        }
    }

    Clouds.colour.a = Alpha0;

}



void Scrollingbackground::render( fboTypes _fbo )
{
    de::Engine::Graphics().add( Clouds, _fbo );
}
