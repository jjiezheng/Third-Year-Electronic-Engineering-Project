#include "AnimatedSprite.h"
#include "CoreEngine.h"

using namespace de::classes;
using namespace de::enums;
using namespace de::theme;

namespace de
{
    namespace graphics
    {

        AnimatedSprite AnimatedSprite::operator= ( const AnimatedSprite &_other )
        {
            animation = _other.animation;
            sprite = _other.sprite;
            return *this;
        }


        AnimatedSprite::AnimatedSprite()
        {
            //ctor
        }

        AnimatedSprite::~AnimatedSprite()
        {
            //dtor
        }



        void AnimatedSprite::addFrames( const std::vector<std::string> &_frames )
        {
            animation.addFrames( de::Engine::Resources().getSprites( _frames ) );
        }

        void AnimatedSprite::addFrame( const std::string &_frame )
        {
            animation.addFrame( de::Engine::Resources().getSprite( _frame ) );
        }

        void AnimatedSprite::setAnimation( de::coords::animation::Type _type, int _timeBetweenFrames )
        {
            animation.cycleType( _type, _timeBetweenFrames );
        }

        void AnimatedSprite::setAnimation( const std::string &_type, int _timeBetweenFrames )
        {
            if( _type == "Forwards" )
                setAnimation( coords::animation::FORWARDS, _timeBetweenFrames );
            else if( _type == "Forwards" )
                setAnimation( coords::animation::FORWARDS, _timeBetweenFrames );
            else if( _type == "Backwards" )
                setAnimation( coords::animation::BACKWARDS, _timeBetweenFrames );
            else if( _type == "ForwardsAndStop" )
                setAnimation( coords::animation::FORWARDS_AND_STOP, _timeBetweenFrames );
            else if( _type == "BackwardsAndStop" )
                setAnimation( coords::animation::BACKWARDS_AND_STOP, _timeBetweenFrames );
            else if( _type == "BackAndForth" )
                setAnimation( coords::animation::BACK_AND_FORTH, _timeBetweenFrames );

            else setAnimation( coords::animation::FORWARDS, _timeBetweenFrames );
        }



        void AnimatedSprite::size( const int &_width, const int &_height )
        {
            sprite.coords.Setup( _width, _height, true );
        }

        void AnimatedSprite::moveBy( const Vector &_movement, bool _moveFromOrign )
        {
            if( _moveFromOrign )
                sprite.coords.transform( _movement - sprite.coords.centre );
            else
                sprite.coords.transform( _movement );
        }

        void AnimatedSprite::moveRotateBy( const Vector &_movement, bool _moveFromOrign )
        {
            if( _moveFromOrign )
                sprite.coords.transformRotate( _movement - sprite.coords.centre );
            else
                sprite.coords.transformRotate( _movement );
        }

        Poly& AnimatedSprite::coords()
        {
            return sprite.coords;
        }



        void AnimatedSprite::setGraphics( const std::string &_texture, const std::string &_shader )
        {
            sprite.setTexture( _texture, _shader );
        }

        void AnimatedSprite::priority( const int &_priority )
        {
            sprite.priority = _priority;
        }



        void AnimatedSprite::logic( const Uint32 &_deltaticks )
        {
            sprite.texCoords = animation.play( _deltaticks );
            //sprite.coords = animation.play( _deltaticks );
        }

        void AnimatedSprite::render( de::enums::fboTypes _layer )
        {
            sprite.render( _layer );
        }

        void AnimatedSprite::reload()
        {
            sprite.reload();
        }
    }
}
