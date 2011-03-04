#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "CoreEnumsAndClasses.h"
#include "Animation.h"

namespace de
{
    namespace graphics
    {
        class AnimatedSprite
        {
            public:
                AnimatedSprite();
                virtual ~AnimatedSprite();

                void addFrame( const std::string &_frame );
                void addFrames( const std::vector<std::string> &_frames );
                void setAnimation( de::coords::animation::Type _type, int _timeBetweenFrames );
                void setAnimation( const std::string &_type, int _timeBetweenFrames );

                void setGraphics( const std::string &_texture, const std::string &_shader );
                void priority( const int &_priority );

                void size( const int &_width, const int &_height );
                void moveBy( const de::classes::Vector &_movement, bool _moveFromOrign = false );
                void moveRotateBy( const de::classes::Vector &_movement, bool _moveFromOrign );
                de::classes::Poly& coords();

                void logic( const Uint32 &_deltaticks );
                void render( de::enums::fboTypes _layer );
                void reload();

                AnimatedSprite operator= ( const AnimatedSprite &_other );

            protected:
            private:
                de::coords::FrameAnimation animation;
                de::classes::Sprite sprite;
        };
    }
}

#endif // ANIMATEDSPRITE_H
