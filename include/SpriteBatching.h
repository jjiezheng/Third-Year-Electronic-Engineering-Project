#ifndef SPRITE_BATCHING_H_
#define SPRITE_BATCHING_H_

#include "Monitor.h"
#include "Settings.h"
#include "Coords.h"

#include <vector>
#include <string>
#include <iostream>



namespace de
{
    namespace internal
    {

        //! Class used by Graphics and Batch internally.
        /*! A single Vertex. Made for use in Batch.
        */
        class Vertex
        {
            public:
                //! Constructor using a Sprite with offset to say which of the four points of the Sprite to take.
                Vertex( const de::classes::Sprite& _sprite, const int &offset );
                //! Constructor using a Poly with offset to say which of the four points of the Poly to take.
                Vertex( const de::classes::Poly& _poly, const de::classes::Colour &_colour, const int &offset );
                //! Constructor using a Vector
                Vertex( const de::classes::Vector& _vector, const de::classes::Colour &_colour );

            public:
                //! The Vertex's position onscreen
                de::classes::Vector position;
                //! Normalised position on the Texture been used.
                de::classes::Vector texture;
                //! Normalised position on the second Texture been used.
                de::classes::Vector texture2;
                //! The Vertex's colour.
                float colour[4];
        };

        //! Class used by Graphics internally.
        /*! A Batch. The Graphics class renders everything with Batches.
        */
        class BatchFixedPipline
        {
            public:
                //! Constructor with Sprite.
                BatchFixedPipline( const de::classes::Sprite &_Sprite );
                //! Constructor with Line.
                BatchFixedPipline( const de::classes::Line &_line );
                //! Constructor with Poly.
                BatchFixedPipline( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Add a Sprite to the Batch.
                void add( const de::classes::Sprite &_Sprite );
                //! Add a Line to the Batch.
                void add( const de::classes::Line &_line );
                //! Add a Poly to the Batch.
                void add( const de::classes::Poly &_poly, const de::classes::Colour &_colour = de::classes::Colour( 1,1,1,1 ) );

                //! Renders the Batch
                void render();


                //! returns true if Batch has no texture.
                bool unTextured();
                //! Clears all Vertices
                void clear();
                //! Returns true if there are no Vertices
                bool empty();


                //! Operator overload. Batches should be sorted by priority.
                bool operator < ( const BatchFixedPipline &_other ) const
                {
                    return ( priority < _other.priority );
                }

                bool operator < ( const de::classes::Line &_Line )
                {
                    return ( priority < _Line.priority );
                }

                bool operator == ( const de::classes::Sprite &_Sprite ) const
                {
                    return ( Texture == _Sprite.getTexture() && Texture2 == _Sprite.Texture2 && priority == _Sprite.priority );
                }

                bool operator == ( const int &_priority ) const
                {
                    return ( priority == _priority );
                }

            private:
                void bindArrays();

            private:
                std::vector<Vertex> Vertices;
                Uint32 Texture;
                Uint32 Texture2;
                int priority;

                int type;
        };

        //! Class used by Graphics internally.
        /*! A Batch. The Graphics class renders everything with Batches.
        */
        class BatchShaders_2_1
        {
            public:
                //! Constructor with Sprite.
                BatchShaders_2_1( const de::classes::Sprite &_Sprite );
                //! Constructor with Line.
                BatchShaders_2_1( const de::classes::Line &_line );
                //! Constructor with Poly.
                BatchShaders_2_1( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Add a Sprite to the Batch.
                void add( const de::classes::Sprite &_Sprite );
                //! Add a Line to the Batch.
                void add( const de::classes::Line &_line );
                //! Add a Poly to the Batch.
                void add( const de::classes::Poly &_poly, const de::classes::Colour &_colour = de::classes::Colour( 1,1,1,1 ) );

                //! Renders the Batch
                void render();


                //! returns true if Batch has no texture.
                bool unTextured();
                //! Clears all Vertices
                void clear();
                //! Returns true if there are no Vertices
                bool empty();


                //! Operator overload. Batches should be sorted by priority.
                bool operator < ( const BatchShaders_2_1 &_other ) const
                {
                    return ( priority < _other.priority );
                }

                bool operator < ( const de::classes::Line &_Line )
                {
                    return ( priority < _Line.priority );
                }

                bool operator == ( const de::classes::Sprite &_Sprite ) const
                {
                    return ( Texture == _Sprite.getTexture() &&
                             Texture2 == _Sprite.Texture2 &&
                             shader.get( graphics::Shader::PROGRAM ) == _Sprite.getShader().get( graphics::Shader::PROGRAM ) &&
                             priority == _Sprite.priority );
                }

                bool operator == ( const int &_priority ) const
                {
                    return ( priority == _priority );
                }

            private:
                void bindArrays();

            private:
                std::vector<Vertex> Vertices;
                de::graphics::Shader shader;
                Uint32 Texture;
                Uint32 Texture2;

                int type;
                int priority;

                struct
                {
                    int Texture;
                    int Texture2;
                } uniforms;
        };


        //! Class used by Graphics internally.
        /*! A Batch. The Graphics class renders everything with Batches.
        */
        class BatchShaders
        {
            public:
                //! Constructor with Sprite.
                BatchShaders( const de::classes::Sprite &_Sprite );
                //! Constructor with Line.
                BatchShaders( const de::classes::Line &_line );
                //! Constructor with Poly.
                BatchShaders( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Add a Sprite to the Batch.
                void add( const de::classes::Sprite &_Sprite );
                //! Add a Line to the Batch.
                void add( const de::classes::Line &_line );
                //! Add a Poly to the Batch.
                void add( const de::classes::Poly &_poly, const de::classes::Colour &_colour = de::classes::Colour( 1,1,1,1 ) );

                //! Renders the Batch
                void render();


                //! returns true if Batch has no texture.
                bool unTextured();
                //! Clears all Vertices
                void clear();
                //! Returns true if there are no Vertices
                bool empty();


                //! Operator overload. Batches should be sorted by priority.
                bool operator < ( const BatchShaders &_other ) const
                {
                    return ( priority < _other.priority );
                }

                bool operator < ( const de::classes::Line &_Line )
                {
                    return ( priority < _Line.priority );
                }

                bool operator == ( const de::classes::Sprite &_Sprite ) const
                {
                    return ( Texture == _Sprite.getTexture() &&
                             Texture2 == _Sprite.Texture2 &&
                             shader.get( graphics::Shader::PROGRAM ) == _Sprite.getShader().get( graphics::Shader::PROGRAM ) &&
                             priority == _Sprite.priority );
                }

                bool operator == ( const int &_priority ) const
                {
                    return ( priority == _priority );
                }

            private:
                void bindArrays();

            private:
                std::vector<Vertex> Vertices;
                de::graphics::Shader shader;
                Uint32 Texture;
                Uint32 Texture2;

                int type;
                int priority;

                struct
                {
                    int Texture;
                    int Texture2;
                } uniforms;
        };



        //! Class used by Graphics internally.
        /*! A Batch. The Graphics class renders everything with Batches.
        */
        class Batch
        {
            public:
                //! Constructor with Sprite.
                Batch( const de::classes::Sprite &_Sprite );
                //! Constructor with Poly.
                Batch( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority, const de::graphics::Shader &_shader );

                //! Add a Sprites points to the Batch.
                void add( const de::classes::Sprite &_Sprite );
                //! Add a Poly points to the Batch.
                void add( const de::classes::Poly &_poly, const de::classes::Colour &_colour = de::classes::Colour( 1,1,1,1 ) );

                //! Operator overload. Batches should be sorted by priority.
                bool operator < ( const Batch& other ) const
                {
                    return ( priority < other.priority );
                }


            public:
                //! Vector of the Batches Vertices.
                std::vector<Vertex> Vertices;
                //! Handle to the Batches Texture.
                Uint32 Texture;
                //! Handle to the Batches second Texture.
                Uint32 Texture2;
                //! Handle to the Batches Shader.
                de::graphics::Shader shader;

                //! Handles to uniforms in the Shader.
                struct
                {
                    int Texture;
                    int Texture2;
                } uniforms;

                //! Used for sorting. The Higher priority batches are rendered later.
                int priority;
        };

        //! Operator overload. Batches and Lines should be sorted by priority.
        bool operator < ( const Batch &_lhs, const de::classes::Line &_rhs );
    }
}

#endif //SPRITE_BATCHING_H_
