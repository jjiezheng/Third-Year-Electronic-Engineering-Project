#ifndef SPRITEBUFFER_H
#define SPRITEBUFFER_H

#include "Coords.h"
#include "Functions.h"
#include "SpriteBatching.h"
#include "CoreGraphics.h"

namespace de
{
    namespace graphics
    {
        class SpriteBufferBase
        {
            public:
                SpriteBufferBase() {}
                virtual ~SpriteBufferBase() {}


                virtual bool add( const de::classes::Sprite &_Sprite ) = 0;
                virtual bool add( const std::vector<de::classes::Sprite> &_Sprites ) = 0;
                virtual bool add( const de::classes::Line &_Line ) = 0;
                virtual bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority ) = 0;

                virtual void render() = 0;
                virtual void clear() = 0;
                virtual int size() = 0;
        };

        class SpriteBuffer : public SpriteBufferBase
        {
            public:
                SpriteBuffer( glVersion::glVersions _type );
                virtual ~SpriteBuffer();

                //! Changes the type of sprite buffer to _type.
                void set( glVersion::glVersions _type );

                //! Add a Sprite to the SpriteBuffer.
                bool add( const de::classes::Sprite &_Sprite );
                //! Add Sprites to the SpriteBuffer.
                bool add( const std::vector<de::classes::Sprite> &_Sprites );
                //! Add a Line to the SpriteBuffer.
                bool add( const de::classes::Line &_Line );
                //! Add a Coloured Polygon to the SpriteBuffer.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Add a Coloured Polygon to the SpriteBuffer.
                void render();
                //! Clear the SpriteBuffer of all Sprites.
                void clear();
                //! Returns the number of sprite batches in the SpriteBuffer.
                int size();


            private:
                SpriteBufferBase *Buffer;
        };



        class SBFixedPipeline : public SpriteBufferBase
        {
            public:
                SBFixedPipeline();
                virtual ~SBFixedPipeline();


                //! Add a Sprite to the SpriteBuffer.
                bool add( const de::classes::Sprite &_Sprite );
                //! Add Sprites to the SpriteBuffer.
                bool add( const std::vector<de::classes::Sprite> &_Sprites );
                //! Add a Line to the SpriteBuffer.
                bool add( const de::classes::Line &_Line );
                //! Add a Coloured Poly to the SpriteBuffer.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Render all batches
                void render();
                //! Clear all Batches
                void clear();
                //! Returns the size
                int size();

            private:
                std::vector<de::internal::BatchFixedPipline> Batches;
                std::vector<de::internal::BatchFixedPipline>::iterator iterBatch, iterBatchEnd;
        };

        class SBShaders_2_1 : public SpriteBufferBase
        {
            public:
                SBShaders_2_1();
                virtual ~SBShaders_2_1();
                //! Add a Sprite to the SpriteBuffer.
                bool add( const de::classes::Sprite &_Sprite );
                //! Add Sprites to the SpriteBuffer.
                bool add( const std::vector<de::classes::Sprite> &_Sprites );
                //! Add a Line to the SpriteBuffer.
                bool add( const de::classes::Line &_Line );
                //! Add a Coloured Poly to the SpriteBuffer.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Render all batches
                void render();
                //! Clear all Batches
                void clear();
                //! Returns the size
                int size();

            private:
                std::vector<de::internal::BatchShaders_2_1> Batches;
                std::vector<de::internal::BatchShaders_2_1>::iterator iterBatch, iterBatchEnd;
        };

        class SBShaders : public SpriteBufferBase
        {
            public:
                SBShaders();
                virtual ~SBShaders();
                //! Add a Sprite to the SpriteBuffer.
                bool add( const de::classes::Sprite &_Sprite );
                //! Add Sprites to the SpriteBuffer.
                bool add( const std::vector<de::classes::Sprite> &_Sprites );
                //! Add a Line to the SpriteBuffer.
                bool add( const de::classes::Line &_Line );
                //! Add a Coloured Poly to the SpriteBuffer.
                bool add( const de::classes::Poly &_poly, const de::classes::Colour &_colour, const int &_priority );

                //! Render all batches
                void render();
                //! Clear all Batches
                void clear();
                //! Returns the size
                int size();

            private:
                std::vector<de::internal::BatchShaders> Batches;
                std::vector<de::internal::BatchShaders>::iterator iterBatch, iterBatchEnd;
        };
    }
}

#endif // SPRITEBUFFER_H
