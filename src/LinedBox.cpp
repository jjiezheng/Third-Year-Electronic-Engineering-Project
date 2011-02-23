#include "LinedBox.h"
#include "CoreEngine.h"

using namespace de::core;
using namespace de::graphics;
using namespace de::enums;


namespace de
{
    namespace classes
    {
        LinedBox::LinedBox()
        {
            edge.priority = 11;
            edge.shader = Engine::Resources().getShader( "Untextured" );


            friendly = Colour( 0.0667, 0.7098,0.0667, 0.65 );
            enemy = Colour( 0.7098, 0.0667,0.0667, 0.65 );
        }

        LinedBox::LinedBox( const Poly &_newPosition )
        {
            edge.priority = 11;
            edge.shader = Engine::Resources().getShader( "Untextured" );


            friendly = Colour( 0.0667, 0.7098,0.0667, 0.65 );
            enemy = Colour( 0.7098, 0.0667,0.0667, 0.65 );


            box = _newPosition;
            edge.Points = box.Vertices;

            if( edge.Points.size() == 4 )
            {
                Vector swap = edge.Points[2];
                edge.Points[2] = edge.Points[3];
                edge.Points[3] = swap;
            }

            edge.Colours.clear();
            for( unsigned int i = 0; i < edge.Points.size(); ++i )
            {
                edge.Colours.push_back( Colour( 0,0,0,1 ) );
            }
        }

        Poly& LinedBox::getPoly()
        {
            return box;
        }



        void LinedBox::update( const Poly &_newPosition )
        {
            box = _newPosition;
            edge.Points = box.Vertices;

            if( edge.Points.size() == 4 )
            {
                Vector swap = edge.Points[2];
                edge.Points[2] = edge.Points[3];
                edge.Points[3] = swap;
            }

            edge.Colours.clear();
            for( unsigned int i = 0; i < edge.Points.size(); ++i )
            {
                edge.Colours.push_back( Colour( 0,0,0,1 ) );
            }
        }


        void LinedBox::render( const de::enums::Collision &_label, const int &_type )
        {
            Engine::Graphics().add( edge, _type );
            if( _label == ENEMY )
                Engine::Graphics().add( box, enemy, 10, _type );
            else if( _label == FRIENDLY )
                Engine::Graphics().add( box, friendly, 10, _type );
            else
                de::io::error << "Wrong label given to Lined Box\n";
        }

        void LinedBox::render( const Colour &_colour, const int &_type )
        {
            Engine::Graphics().add( edge, _type );
            Engine::Graphics().add( box, _colour, 10, _type );
        }

    }
}
