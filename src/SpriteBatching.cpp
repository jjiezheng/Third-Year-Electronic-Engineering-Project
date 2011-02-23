#include "SpriteBatching.h"
#include "GameServices.h"
#include "openGL.h"

using namespace de::classes;
using namespace de::enums;


namespace de
{
    namespace internal
    {
        Vertex::Vertex( const de::classes::Sprite& _sprite, const int &offset )
        {
            position = _sprite.coords.Vertices[offset];

            colour[0] = _sprite.colour.r;
            colour[1] = _sprite.colour.g;
            colour[2] = _sprite.colour.b;
            colour[3] = _sprite.colour.a;


            texture = _sprite.texCoords.Vertices[offset];
            texture = _sprite.texCoords.Vertices[offset];

            texture2 = _sprite.texCoords2.Vertices[offset];
            texture2 = _sprite.texCoords2.Vertices[offset];
        }

        Vertex::Vertex( const de::classes::Poly& _poly, const de::classes::Colour &_colour, const int &offset )
        {
            position = _poly.Vertices[offset];

            colour[0] = _colour.r;
            colour[1] = _colour.g;
            colour[2] = _colour.b;
            colour[3] = _colour.a;
        }

        Vertex::Vertex( const de::classes::Vector& _vector, const de::classes::Colour &_colour )
        {
            position = _vector;

            colour[0] = _colour.r;
            colour[1] = _colour.g;
            colour[2] = _colour.b;
            colour[3] = _colour.a;
        }






        BatchFixedPipline::BatchFixedPipline( const Sprite &_Sprite )
        {
            //Vertices.reserve( 100 );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );

            Texture = _Sprite.getTexture();
            Texture2 = _Sprite.Texture2;
            priority = _Sprite.priority;
            type = GL_TRIANGLE_STRIP;
        }

        BatchFixedPipline::BatchFixedPipline( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );

            Texture = 0;
            Texture2 = 0;
            priority = _priority;
            type = GL_TRIANGLE_STRIP;
        }

        BatchFixedPipline::BatchFixedPipline( const de::classes::Line &_line )
        {
            std::vector<Vector>::const_iterator VectorIter, end = _line.Points.end();
            std::vector<Colour>::const_iterator ColourIter;

            for( VectorIter = _line.Points.begin(), ColourIter = _line.Colours.begin(); VectorIter != end; ++VectorIter, ++ColourIter )
            {
                Vertices.push_back( Vertex( (*VectorIter), (*ColourIter) ) );
            }

            Texture = 0;
            Texture2 = 0;
            priority = _line.priority;
            type = GL_LINE_STRIP;
        }



        void BatchFixedPipline::add( const Sprite &_Sprite )
        {
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
        }

        void BatchFixedPipline::add( const Poly &_poly, const Colour &_colour )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
        }



        bool BatchFixedPipline::unTextured()
        {
            if( Texture == 0 )
                return true;

            return false;
        }

        void BatchFixedPipline::render()
        {
            if( !Vertices.empty() )
            {
                bindArrays();
                CHECKGL( glPushMatrix() );
                    CHECKGL( glDrawArrays( type, 0, Vertices.size() ) );
                CHECKGL( glPopMatrix() );

                CHECKGL( glDisableClientState( GL_COLOR_ARRAY ) );
                CHECKGL( glDisableClientState( GL_VERTEX_ARRAY ) );
                CHECKGL( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
            }
        }


        void BatchFixedPipline::clear()
        {
            Vertices.clear();
        }

        bool BatchFixedPipline::empty()
        {
            return Vertices.empty();
        }


        void BatchFixedPipline::bindArrays()
        {
            CHECKGL( glVertexPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].position.x ) );
            CHECKGL( glEnableClientState( GL_VERTEX_ARRAY ) );

            CHECKGL( glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &Vertices[0].colour ) );
            CHECKGL( glEnableClientState( GL_COLOR_ARRAY ) );

            if( Texture )
            {
                CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].texture.x ) );
                CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

                CHECKGL( glEnable( GL_TEXTURE_2D ) );
                CHECKGL( glActiveTexture( GL_TEXTURE0 ) );

                CHECKGL( glBindTexture( GL_TEXTURE_2D, Texture ) );

            }
            else CHECKGL( glDisable( GL_TEXTURE_2D ) );

        }







        BatchShaders_2_1::BatchShaders_2_1( const Sprite &_Sprite )
        {
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );

            Texture = _Sprite.getTexture();
            Texture2 = _Sprite.Texture2;
            shader = _Sprite.getShader();
            priority = _Sprite.priority;
            type = GL_TRIANGLE_STRIP;
        }

        BatchShaders_2_1::BatchShaders_2_1( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );

            Texture = 0;
            Texture2 = 0;
            shader = Engine::Resources().getShader( "Untextured" );
            priority = _priority;
            type = GL_TRIANGLE_STRIP;
        }

        BatchShaders_2_1::BatchShaders_2_1( const de::classes::Line &_line )
        {
            std::vector<Vector>::const_iterator VectorIter, end = _line.Points.end();
            std::vector<Colour>::const_iterator ColourIter;

            for( VectorIter = _line.Points.begin(), ColourIter = _line.Colours.begin(); VectorIter != end; ++VectorIter, ++ColourIter )
            {
                Vertices.push_back( Vertex( (*VectorIter), (*ColourIter) ) );
            }

            Texture = 0;
            Texture2 = 0;
            shader = Engine::Resources().getShader( "Untextured" );
            priority = _line.priority;
            type = GL_LINE_STRIP;
        }



        void BatchShaders_2_1::add( const Sprite &_Sprite )
        {
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
        }

        void BatchShaders_2_1::add( const Poly &_poly, const Colour &_colour )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
        }



        bool BatchShaders_2_1::unTextured()
        {
            if( Texture == 0 )
                return true;

            return false;
        }

        void BatchShaders_2_1::render()
        {
            if( !Vertices.empty() )
            {
                if( shader.get( graphics::Shader::PROGRAM ) )
                {
                    shader.bindShader();
                }
                else CHECKGL( glUseProgram( 0 ) );


                bindArrays();
                CHECKGL( glPushMatrix() );
                    CHECKGL( glDrawArrays( type, 0, Vertices.size() ) );
                CHECKGL( glPopMatrix() );

                CHECKGL( glDisableClientState( GL_COLOR_ARRAY ) );
                CHECKGL( glDisableClientState( GL_VERTEX_ARRAY ) );
                CHECKGL( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
            }
        }


        void BatchShaders_2_1::clear()
        {
            Vertices.clear();
        }

        bool BatchShaders_2_1::empty()
        {
            return Vertices.empty();
        }


        void BatchShaders_2_1::bindArrays()
        {
            CHECKGL( glVertexPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].position.x ) );
            CHECKGL( glEnableClientState( GL_VERTEX_ARRAY ) );

            CHECKGL( glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &Vertices[0].colour ) );
            CHECKGL( glEnableClientState( GL_COLOR_ARRAY ) );

            if( Texture )
            {
                CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].texture.x ) );
                CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

                CHECKGL( glEnable( GL_TEXTURE_2D ) );
                CHECKGL( glClientActiveTexture( GL_TEXTURE0 ) );
                CHECKGL( glActiveTexture( GL_TEXTURE0 ) );

                CHECKGL( glBindTexture( GL_TEXTURE_2D, Texture ) );
/*
                if( Texture2 )
                {
                    CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].texture2.x ) );
                    CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

                    CHECKGL( glEnable( GL_TEXTURE_2D ) );
                    CHECKGL( glClientActiveTexture( GL_TEXTURE1 ) );
                    CHECKGL( glActiveTexture( GL_TEXTURE1 ) );

                    CHECKGL( glBindTexture( GL_TEXTURE_2D, Texture2 ) );
                }*/
            }
            else CHECKGL( glDisable( GL_TEXTURE_2D ) );

        }




        BatchShaders::BatchShaders( const Sprite &_Sprite )
        {
            //Vertices.reserve( 100 );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );

            Texture = _Sprite.getTexture();
            Texture2 = _Sprite.Texture2;
            shader = _Sprite.getShader();
            priority = _Sprite.priority;
            type = GL_TRIANGLE_STRIP;
        }

        BatchShaders::BatchShaders( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );

            Texture = 0;
            Texture2 = 0;
            shader = Engine::Resources().getShader( "Untextured" );
            priority = _priority;
            type = GL_TRIANGLE_STRIP;
        }

        BatchShaders::BatchShaders( const de::classes::Line &_line )
        {
            std::vector<Vector>::const_iterator VectorIter, end = _line.Points.end();
            std::vector<Colour>::const_iterator ColourIter;

            for( VectorIter = _line.Points.begin(), ColourIter = _line.Colours.begin(); VectorIter != end; ++VectorIter, ++ColourIter )
            {
                Vertices.push_back( Vertex( (*VectorIter), (*ColourIter) ) );
            }

            Texture = 0;
            Texture2 = 0;
            shader = Engine::Resources().getShader( "Untextured" );
            priority = _line.priority;
            type = GL_LINE_STRIP;
        }



        void BatchShaders::add( const Sprite &_Sprite )
        {
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
        }

        void BatchShaders::add( const Poly &_poly, const Colour &_colour )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
        }



        bool BatchShaders::unTextured()
        {
            if( Texture == 0 )
                return true;

            return false;
        }

        void BatchShaders::render()
        {
            if( !Vertices.empty() )
            {
                if( shader.get( graphics::Shader::PROGRAM ) )
                {
                    shader.bindShader();
                }
                else CHECKGL( glUseProgram( 0 ) );


                bindArrays();
                CHECKGL( glPushMatrix() );
                    CHECKGL( glDrawArrays( type, 0, Vertices.size() ) );
                CHECKGL( glPopMatrix() );

                CHECKGL( glDisableClientState( GL_COLOR_ARRAY ) );
                CHECKGL( glDisableClientState( GL_VERTEX_ARRAY ) );
                CHECKGL( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
            }
        }


        void BatchShaders::clear()
        {
            Vertices.clear();
        }

        bool BatchShaders::empty()
        {
            return Vertices.empty();
        }


        void BatchShaders::bindArrays()
        {
            CHECKGL( glVertexPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].position.x ) );
            CHECKGL( glEnableClientState( GL_VERTEX_ARRAY ) );

            CHECKGL( glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &Vertices[0].colour ) );
            CHECKGL( glEnableClientState( GL_COLOR_ARRAY ) );

            if( Texture )
            {
                CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].texture.x ) );
                CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

                CHECKGL( glEnable( GL_TEXTURE_2D ) );
                CHECKGL( glClientActiveTexture( GL_TEXTURE0 ) );
                CHECKGL( glActiveTexture( GL_TEXTURE0 ) );

                CHECKGL( glBindTexture( GL_TEXTURE_2D, Texture ) );
/*
                if( Texture2 )
                {
                    CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &Vertices[0].texture2.x ) );
                    CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

                    CHECKGL( glEnable( GL_TEXTURE_2D ) );
                    CHECKGL( glClientActiveTexture( GL_TEXTURE1 ) );
                    CHECKGL( glActiveTexture( GL_TEXTURE1 ) );

                    CHECKGL( glBindTexture( GL_TEXTURE_2D, Texture2 ) );
                }*/
            }
            else CHECKGL( glDisable( GL_TEXTURE_2D ) );

        }













        bool operator < ( const Batch &_lhs, const Line &_rhs )
        {
            return ( _lhs.priority < _rhs.priority );
        }

        Batch::Batch( const Sprite &_Sprite )
        {
            //Vertices.reserve( 100 );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );

            Texture = _Sprite.getTexture();
            Texture2 = _Sprite.Texture2;
            shader = _Sprite.getShader();

            priority = _Sprite.priority;


            CHECKGL( uniforms.Texture = glGetUniformLocation( shader.get( graphics::Shader::PROGRAM ), "Texture0" ) );
            CHECKGL( uniforms.Texture2 = glGetUniformLocation( shader.get( graphics::Shader::PROGRAM ), "Texture1" ) );
        }

        Batch::Batch( const Poly &_poly, const Colour &_colour, const int &_priority, const de::graphics::Shader &_shader )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );

            Texture = 0;
            Texture2 = 0;
            priority = _priority;
            shader = _shader;

            uniforms.Texture = glGetUniformLocation( shader.get( graphics::Shader::PROGRAM ), "Texture0" );
            uniforms.Texture2 = glGetUniformLocation( shader.get( graphics::Shader::PROGRAM ), "Texture1" );
        }

        void Batch::add( const Sprite &_Sprite )
        {
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 0 ) );
            Vertices.push_back( Vertex( _Sprite, 1 ) );
            Vertices.push_back( Vertex( _Sprite, 2 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
            Vertices.push_back( Vertex( _Sprite, 3 ) );
        }

        void Batch::add( const Poly &_poly, const Colour &_colour )
        {
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 0 ) );
            Vertices.push_back( Vertex( _poly, _colour, 1 ) );
            Vertices.push_back( Vertex( _poly, _colour, 2 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
            Vertices.push_back( Vertex( _poly, _colour, 3 ) );
        }

    }
}

