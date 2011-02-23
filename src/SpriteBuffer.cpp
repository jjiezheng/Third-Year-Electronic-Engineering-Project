#include "SpriteBuffer.h"

#include <algorithm>

using namespace de::classes;
using namespace de::internal;
using namespace de::enums;

namespace de
{
    namespace graphics
    {
        SpriteBuffer::SpriteBuffer( glVersion::glVersions _type ) : Buffer(NULL)
        {
            set( _type );
        }

        SpriteBuffer::~SpriteBuffer()
        {
            if( Buffer ) delete Buffer;
        }



        void SpriteBuffer::set( glVersion::glVersions _type )
        {
            if( Buffer ) delete Buffer;

            if( _type == glVersion::GL_1_1 ||
                _type == glVersion::GL_1_2 ||
                _type == glVersion::GL_1_3 ||
                _type == glVersion::GL_1_4 ||
                _type == glVersion::GL_1_5  )
            {
                Buffer = new SBFixedPipeline();
            }
            else if( _type == glVersion::GL_2_0 ||
                     _type == glVersion::GL_2_1 )
            {
                Buffer = new SBShaders_2_1();
            }
            else if( _type == glVersion::GL_3_0 ||
                     _type == glVersion::GL_3_1 ||
                     _type == glVersion::GL_3_2 ||
                     _type == glVersion::GL_3_3  )
            {
                Buffer = new SBShaders();
            }
            else if( _type == glVersion::GL_4_0 ||
                     _type == glVersion::GL_4_1 ||
                     _type > glVersion::GL_4_1 )
            {
                new SBShaders();
            }
        }

        bool SpriteBuffer::add( const std::vector<Sprite> &_Sprites )
        {
            return Buffer->add( _Sprites );
        }

        bool SpriteBuffer::add( const Sprite &_Sprite )
        {
            return Buffer->add( _Sprite );
        }

        bool SpriteBuffer::add( const Line &_Line )
        {
            return Buffer->add( _Line );
        }

        bool SpriteBuffer::add( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            return Buffer->add( _poly, _colour, _priority );
        }




        void SpriteBuffer::render()
        {
            Buffer->render();
        }

        void SpriteBuffer::clear()
        {
            Buffer->clear();
        }

        int SpriteBuffer::size()
        {
            return Buffer->size();
        }








        SBFixedPipeline::SBFixedPipeline()
        {
            //ctor
        }

        SBFixedPipeline::~SBFixedPipeline()
        {
            //dtor
        }


        bool SBFixedPipeline::add( const std::vector<Sprite> &_Sprites )
        {
            std::vector<Sprite>::const_iterator iter, end = _Sprites.end();;
            for( iter = _Sprites.begin(); iter < end; ++iter )
            {
                add( (*iter));
            }
            return true;
        }

        bool SBFixedPipeline::add( const Sprite &_Sprite )
        {
            iterBatchEnd = Batches.end();
            for( iterBatch = Batches.begin(); iterBatch < Batches.end(); ++iterBatch )
            {
                if( (*iterBatch) == _Sprite )
                {
                    iterBatch->add( _Sprite );
                    return true;
                }
            }
            Batches.push_back( BatchFixedPipline( _Sprite ) );
            return true;
        }

        bool SBFixedPipeline::add( const Line &_Line )
        {
            Batches.push_back( BatchFixedPipline( _Line ) );
            return true;
        }

        bool SBFixedPipeline::add( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            for( iterBatch = Batches.begin(); iterBatch < Batches.end(); ++iterBatch )
            {
                if( iterBatch->unTextured() && (*iterBatch) == _priority )
                {
                    iterBatch->add( _poly, _colour );
                    return true;
                }
            }
            Batches.push_back( BatchFixedPipline( _poly, _colour, _priority ) );
            return true;
        }


        void SBFixedPipeline::render()
        {
            sort( Batches.begin(), Batches.end() );
            iterBatch = Batches.begin();
            iterBatchEnd = Batches.end();

            while( iterBatch != iterBatchEnd )
            {
                if( !iterBatch->empty() )
                {
                    iterBatch->render();
                    iterBatch->clear();
                    ++iterBatch;
                }
                else
                {
                    iterBatch = Batches.erase( iterBatch );
                    iterBatchEnd = Batches.end();
                }
            }
        }

        void SBFixedPipeline::clear()
        {
            Batches.clear();
        }

        int SBFixedPipeline::size()
        {
            return Batches.size();
        }





        SBShaders_2_1::SBShaders_2_1()
        {
            //ctor
        }

        SBShaders_2_1::~SBShaders_2_1()
        {
            //dtor
        }


        bool SBShaders_2_1::add( const std::vector<Sprite> &_Sprites )
        {
            std::vector<Sprite>::const_iterator iter, end = _Sprites.end();;
            for( iter = _Sprites.begin(); iter < end; ++iter )
            {
                add( (*iter));
            }
            return true;
        }

        bool SBShaders_2_1::add( const Sprite &_Sprite )
        {
            iterBatchEnd = Batches.end();
            for( iterBatch = Batches.begin(); iterBatch < Batches.end(); ++iterBatch )
            {
                if( (*iterBatch) == _Sprite )
                {
                    iterBatch->add( _Sprite );
                    return true;
                }
            }
            Batches.push_back( BatchShaders_2_1( _Sprite ) );
            return true;
        }

        bool SBShaders_2_1::add( const Line &_Line )
        {
            Batches.push_back( BatchShaders_2_1( _Line ) );
            return true;
        }

        bool SBShaders_2_1::add( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            for( iterBatch = Batches.begin(); iterBatch < Batches.end(); ++iterBatch )
            {
                if( iterBatch->unTextured() && (*iterBatch) == _priority )
                {
                    iterBatch->add( _poly, _colour );
                    return true;
                }
            }
            Batches.push_back( BatchShaders_2_1( _poly, _colour, _priority ) );
            return true;
        }


        void SBShaders_2_1::render()
        {
            sort( Batches.begin(), Batches.end() );
            iterBatch = Batches.begin();
            iterBatchEnd = Batches.end();

            while( iterBatch != iterBatchEnd )
            {
                if( !iterBatch->empty() )
                {
                    iterBatch->render();
                    iterBatch->clear();
                    ++iterBatch;
                }
                else
                {
                    iterBatch = Batches.erase( iterBatch );
                    iterBatchEnd = Batches.end();
                }
            }
        }

        void SBShaders_2_1::clear()
        {
            Batches.clear();
        }

        int SBShaders_2_1::size()
        {
            return Batches.size();
        }



/*
        void SBShaders_20::renderBatch( std::vector<Line> &_LineBatch )
        {
            CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );

            //Vertex array rendering of Textured Quads
            iterLine = _LineBatch.begin();
            while ( iterLine != _LineBatch.end() )
            {
                renderBatch( (*iterLine) );
                ++iterLine;
            }

            _LineBatch.clear();
        }

        void SBShaders_20::renderBatch( std::vector<Batch> &_Batch )
        {
            CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );

            //Vertex array rendering of Textured Quads
            iterBatch = _Batch.begin();
            while ( iterBatch != _Batch.end() )
            {
                if( !iterBatch->Vertices.empty() )
                {
                    renderBatch( (*iterBatch) );

                    iterBatch->Vertices.clear();
                    ++iterBatch;
                }
                else
                {
                    iterBatch = _Batch.erase( iterBatch );
                }
            }
        }


        void SBShaders_20::renderBatch( Batch &_Batch )
        {
            if( iterBatch->shader.shaderHandle )
            {
                iterBatch->shader.bindShader();
            }
            else CHECKGL( glUseProgram( 0 ) );

            bindArrays( _Batch );
            CHECKGL( glPushMatrix() );
                CHECKGL( glDrawArrays( GL_TRIANGLE_STRIP, 0, _Batch.Vertices.size() ) );
            CHECKGL( glPopMatrix() );

            CHECKGL( glDisableClientState( GL_COLOR_ARRAY ) );
            CHECKGL( glDisableClientState( GL_VERTEX_ARRAY ) );
            CHECKGL( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );

        }

        void SBShaders_20::renderBatch( Line &_LineBatch )
        {
            if( _LineBatch.shader.shaderHandle )
            {
                _LineBatch.shader.bindShader();
            }
            else CHECKGL( glUseProgram( 0 ) );

            bindArrays( _LineBatch );
            CHECKGL( glPushMatrix() );
                CHECKGL( glDrawArrays( _LineBatch.type, 0, _LineBatch.Points.size() ) );
            CHECKGL( glPopMatrix() );

            CHECKGL( glDisableClientState( GL_COLOR_ARRAY ) );
            CHECKGL( glDisableClientState( GL_VERTEX_ARRAY ) );
            CHECKGL( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
        }



        void SBShaders_20::bindArrays( const Batch &_batch )
        {
            CHECKGL( glVertexPointer( 2, GL_FLOAT, sizeof(Vertex), &iterBatch->Vertices[0].position.x ) );
            CHECKGL( glEnableClientState( GL_VERTEX_ARRAY ) );

            CHECKGL( glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &iterBatch->Vertices[0].colour ) );
            CHECKGL( glEnableClientState( GL_COLOR_ARRAY ) );



            if( iterBatch->Texture )
            {
                CHECKGL( glClientActiveTexture( GL_TEXTURE0 ) );
                CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &iterBatch->Vertices[0].texture.x ) );
                CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );


                if( iterBatch->Texture2 )
                {
                    CHECKGL( glClientActiveTexture( GL_TEXTURE1 ) );
                    CHECKGL( glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &iterBatch->Vertices[0].texture2.x ) );
                    CHECKGL( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );
                }

                CHECKGL( glActiveTexture( GL_TEXTURE0 ) );
                CHECKGL( glClientActiveTexture( GL_TEXTURE0 ) );
                CHECKGL( glEnable( GL_TEXTURE_2D ) );

                CHECKGL( glBindTexture( GL_TEXTURE_2D, iterBatch->Texture ) );
                CHECKGL( glUniform1i( iterBatch->uniforms.Texture, 0 ) );


                if( iterBatch->Texture2 )
                {
                    CHECKGL( glActiveTexture (GL_TEXTURE1 ) );
                    CHECKGL( glClientActiveTexture( GL_TEXTURE1 ) );
                    CHECKGL( glEnable( GL_TEXTURE_2D ) );

                    CHECKGL( glBindTexture( GL_TEXTURE_2D, iterBatch->Texture2 ) );
                    CHECKGL( glUniform1i( iterBatch->uniforms.Texture2, 1 ) );
                }
            }
        }

        void SBShaders_20::bindArrays( const Line &_lineBatch )
        {
            CHECKGL( glVertexPointer( 2, GL_FLOAT, sizeof(Vector), &_lineBatch.Points[0].x ) );
            CHECKGL( glEnableClientState( GL_VERTEX_ARRAY ) );

            CHECKGL( glColorPointer( 4, GL_FLOAT, sizeof(Colour), &_lineBatch.Colours[0].r ) );
            CHECKGL( glEnableClientState( GL_COLOR_ARRAY ) );
        }

*/







        SBShaders::SBShaders()
        {
            //ctor
        }

        SBShaders::~SBShaders()
        {
            //dtor
        }

        bool SBShaders::add( const std::vector<Sprite> &_Sprites )
        {
            std::vector<Sprite>::const_iterator iter, end = _Sprites.end();;
            for( iter = _Sprites.begin(); iter < end; ++iter )
            {
                add( (*iter));
            }
            return true;
        }

        bool SBShaders::add( const Sprite &_Sprite )
        {
            iterBatchEnd = Batches.end();
            for( iterBatch = Batches.begin(); iterBatch < Batches.end(); ++iterBatch )
            {
                if( (*iterBatch) == _Sprite )
                {
                    iterBatch->add( _Sprite );
                    return true;
                }
            }
            Batches.push_back( BatchShaders( _Sprite ) );
            return true;
        }

        bool SBShaders::add( const Line &_Line )
        {
            Batches.push_back( BatchShaders( _Line ) );
            return true;
        }

        bool SBShaders::add( const Poly &_poly, const Colour &_colour, const int &_priority )
        {
            for( iterBatch = Batches.begin(); iterBatch < Batches.end(); ++iterBatch )
            {
                if( iterBatch->unTextured() && (*iterBatch) == _priority )
                {
                    iterBatch->add( _poly, _colour );
                    return true;
                }
            }
            Batches.push_back( BatchShaders( _poly, _colour, _priority ) );
            return true;
        }


        void SBShaders::render()
        {
            sort( Batches.begin(), Batches.end() );
            iterBatch = Batches.begin();
            iterBatchEnd = Batches.end();

            while( iterBatch != iterBatchEnd )
            {
                if( !iterBatch->empty() )
                {
                    iterBatch->render();
                    iterBatch->clear();
                    ++iterBatch;
                }
                else
                {
                    iterBatch = Batches.erase( iterBatch );
                    iterBatchEnd = Batches.end();
                }
            }
        }

        void SBShaders::clear()
        {
            Batches.clear();
        }

        int SBShaders::size()
        {
            return Batches.size();
        }


    }
}

