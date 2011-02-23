#include "Animation.h"



namespace de
{
    namespace classes
    {
        Animation::Animation()
            :cachePosition(0)
            ,second(false), halfway(false), fullyCached(false)
        {
        }



        void Animation::setup( const float &_x, const float &_y )
        {
            texCoords.Setup( _x, _y );

            AnimationCache.clear();
            AnimationCache.push_back( texCoords );
        }

        void Animation::setup( const Frect &_texRext )
        {
            texCoords.Setup( _texRext, 0 );
            start = Vector( _texRext.x, _texRext.y );

            AnimationCache.clear();
            AnimationCache.push_back( texCoords );
        }



        //Use when the sprites are in a line on the spritesheet
        void Animation::addAnimation( const Vector &_displacement, int _delay, int _reset )
        {
            displacement = _displacement;

            counter.countDelayIs( _delay );
            counter.countResetIs( _reset );
        }

        //Use when the sprites are in rows and columns on the spritesheet
        void Animation::addAnimation2v( const Vector &_first, const Vector &_second, int _delay, int _reset, int _reset2nd )
        {
            second = true;
            displacement = _first;
            displacement2nd = _second;

            counter.countDelayIs( _delay );
            counter.countResetIs( _reset );
            counter2nd.countResetIs( _reset2nd );
        }




        const Poly& Animation::forwards( const Uint32 &_deltaTicks )
        {
            if( !fullyCached )
            {
                if( counter.count( _deltaTicks ) )
                {
                    if( counter.checkReset() )
                    {
                        texCoords.transform( -texCoords.centre + start );

                        if( second )
                        {
                            if( counter2nd.count() )
                                fullyCached = true;

                            for( int i = 0; i < counter2nd.getCurrentCount(); i++ )
                                texCoords.transform( displacement2nd );
                        }
                        else
                            fullyCached = true;
                    }
                    else
                    {
                        texCoords.transform( displacement );
                        AnimationCache.push_back( texCoords );
                    }
                }
                return texCoords;
            }

            if( counter.count( _deltaTicks ) )
            {
               ++cachePosition;

                if( cachePosition >= AnimationCache.size() )
                    cachePosition = 0;
            }
            return AnimationCache[cachePosition];
        }

        const Poly& Animation::loop( const Uint32 &_deltaTicks )
        {
            if( !fullyCached )
            {
                if( counter.count( _deltaTicks ) )
                {
                    if( counter.checkReset() )
                    {
                        displacement = -displacement;
                        if( halfway == false )
                            halfway = true;
                        else
                            fullyCached = true;
                    }
                    else
                    {
                        texCoords.transform( displacement );
                        AnimationCache.push_back( texCoords );
                    }
                }

                return texCoords;
            }

            if( counter.count( _deltaTicks ) )
            {
               ++cachePosition;

                if( cachePosition >= AnimationCache.size() )
                    cachePosition = 0;
            }
            return AnimationCache[cachePosition];
        }

    }

    namespace coords
    {
        FrameAnimation FrameAnimation::operator = ( const FrameAnimation &_other )
        {
            frameGap = _other.frameGap;
            Frames = _other.Frames;

            currentFrame = Frames.begin();

            type = _other.type;
            forward = _other.forward;


            return *this;
        }


        void FrameAnimation::cycleType( animation::Type _type, int _timeBetweenFrames )
        {
            type = _type;
            frameGap.countDelayIs( _timeBetweenFrames );
        }

        void FrameAnimation::addFrame( const de::classes::Poly &_coords )
        {
            Frames.push_back( _coords );
            currentFrame = Frames.begin();
        }

        void FrameAnimation::addFrames( const std::vector<de::classes::Poly> &_Coords )
        {
            for( std::vector<de::classes::Poly>::const_iterator iter = _Coords.begin(); iter < _Coords.end(); ++iter )
                addFrame( (*iter) );
        }

        const de::classes::Poly& FrameAnimation::play( const Uint32 &_deltaTicks )
        {
            if( Frames.empty())
            {
                static de::classes::Poly nothing(0,0,true);
                return nothing;
            }

            if( currentFrame < Frames.begin() || currentFrame > Frames.end() )
            {
                currentFrame = Frames.begin();
            }



            if( frameGap.count( _deltaTicks ) )
            {
                if( type == animation::FORWARDS || type == animation::FORWARDS_AND_STOP )
                    forwards();

                else if( type == animation::BACKWARDS || type == animation::BACKWARDS_AND_STOP )
                    backwards();

                else if( type == animation::BACK_AND_FORTH )
                    backAndForth();
            }
            return (*currentFrame);
        }

        inline void FrameAnimation::forwards()
        {
            if( currentFrame < Frames.end() - 1 )
                ++currentFrame;
            else if( type == animation::FORWARDS )
                currentFrame = Frames.begin();
        }

        inline void FrameAnimation::backwards()
        {
            if( currentFrame > Frames.begin() )
                --currentFrame;
            else if( type == animation::BACKWARDS )
                currentFrame = Frames.end() - 1;
        }

        inline void FrameAnimation::backAndForth()
        {
            if( forward )
            {
                if( currentFrame < Frames.end() - 1 )
                    ++currentFrame;
                else
                {
                    forward = !forward;
                    --currentFrame;
                }
            }
            else
            {
                if( currentFrame > Frames.begin() )
                    --currentFrame;
                else
                {
                    forward = !forward;
                    ++currentFrame;
                }
            }
        }


        inline void FrameAnimation::restart()
        {
            if( type == animation::FORWARDS || type == animation::FORWARDS_AND_STOP || type == animation::BACK_AND_FORTH )
            {
                currentFrame = Frames.begin();
            }
            else if( type == animation::BACKWARDS || type == animation::BACKWARDS_AND_STOP )
            {
                currentFrame = Frames.end() - 1;
            }
        }

        inline void FrameAnimation::clear()
        {
            Frames.clear();
        }

    }

}
