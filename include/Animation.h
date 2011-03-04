#ifndef ANIMATION_H
#define ANIMATION_H

#include "CoreEnumsAndClasses.h"
#include "Vector.h"
#include "Counter.h"

#include <vector>


namespace de
{
    namespace classes
    {

        //! Class used for animating Textures.
        /*! Changes the Textures Coords to achieve Sprite Animation. Animation is Cached so after a full loop it will require less cpu.
        */
        class Animation
        {
            public:
                //! Bacis Constructor
                Animation();
                //! Bacis Destructor
                ~Animation() {};

                //! Setup the Texture coords.
                void setup( const Frect &_texRext );
                //! Setup the Texture coords. Marked for Deletion
                void setup( const float &_x, const float &_y );

                //! Add an animation.
                /*! The Texture coords will be transformed by "_displacement" for every frame of animation.
                    "_delay" is how long to wait in milliseconds for each frame of animation.
                    "_reset" is how long to wait till the Animation is reset.
                */
                void addAnimation(
                        const Vector &_displacement,
                        int _delay,
                        int _reset );

                //! Add an animation.
                /*! Much like addAnimation but has a transform in a second direction after each "_reset". Animation is fully reset when "_reset2nd" is reached.
                */
                void addAnimation2v(
                        const Vector &_first,
                        const Vector &_second,
                        int _delay,
                        int _reset,
                        int _reset2nd );

                //! Move the Animation forwards by "_deltaTicks" milliseconds.
                const Poly& forwards( const Uint32 &_deltaTicks );
                //! Move the Animation forwards by "_deltaTicks" milliseconds but loop the animation.
                const Poly& loop( const Uint32 &_deltaTicks );

            private:
                //! How the Texture coord Poly should move for each frame of animation.
                Vector displacement;
                //! How the Texture coord Poly should move when it needs to move in the second direction.
                Vector displacement2nd;



                time::Counter counter, counter2nd;

                Vector start;
                Poly texCoords;
                Vector texCoord[6];

                std::vector<Poly> AnimationCache;
                unsigned int cachePosition;

                int current, currentState;
                bool second, halfway, fullyCached;
        };
    }


    namespace coords
    {
        namespace animation
        {
            enum Type
            {
                NONE = 0,
                FORWARDS,
                BACKWARDS,
                FORWARDS_AND_STOP,
                BACKWARDS_AND_STOP,
                BACK_AND_FORTH
            };
        }

        class FrameAnimation
        {
            public:
                //! Bacis Constructor
                FrameAnimation() : type(animation::NONE), forward(true) {};
                //! Bacis Destructor
                ~FrameAnimation() {};

                void cycleType( animation::Type _type = animation::NONE, int _timeBetweenFrames = 10 );
                void addFrame( const de::classes::Poly &_coords );
                void addFrames( const std::vector<de::classes::Poly> &_Coords );

                const de::classes::Poly& play( const Uint32 &_deltaTicks );

                inline void restart();
                inline void clear();

                FrameAnimation operator= ( const FrameAnimation &_other );

            private:
                inline void forwards();
                inline void backwards();
                inline void backAndForth();

                de::time::Counter frameGap;
                std::vector<de::classes::Poly> Frames;
                std::vector<de::classes::Poly>::iterator currentFrame;
                animation::Type type;

                bool forward;
        };
/*
        class MovementAnimation
        {
            public:
                //! Bacis Constructor
                MovementAnimation() : type(animation::NONE), forward(true) {};
                //! Bacis Destructor
                ~MovementAnimation() {};

                void cycleType( animation::Type _type = animation::NONE, int _timeBetweenFrames = 10 );
                void addMovement( const de::classes::Poly &_coords, int _startDelay, int _animationTime );

                const de::classes::Poly& play( const Uint32 &_deltaTicks );

                inline void restart();
                inline void clear();
            private:
                inline void forwards();
                inline void backwards();
                inline void backAndForth();

                de::time::Counter frameGap;
                std::vector<de::classes::Poly> Frames;
                std::vector<de::classes::Poly>::iterator currentFrame;
                animation::Type type;

                bool forward;
        };*/
    }
}


#endif //ANIMATION_H
