#include "ColourBlender.h"

using namespace de::classes;


namespace
{
    float lin( const float &_a , const float &_b, const float &_r )
    {
        return ( _a + ( _b - _a) * _r );
    }
}

namespace de
{
    namespace classes
    {
        ColourBlender::ColourBlender() :
            forward(true), totalTime(0), currentTime(0)
        {}

        ColourBlender::ColourBlender( const Colour &_from, const Colour &_to, unsigned int _time )
            :forward(true), totalTime(0), currentTime(0)
        {
            Setup( _from, _to, _time );
        }

        ColourBlender::~ColourBlender()
        {
            //dtor
        }

        void ColourBlender::Setup( const Colour &_from, const Colour &_to, unsigned int _time )
        {
            forward = true;
            currentTime = 0;
            totalTime = _time;

            first = _from;
            second = _to;
            current = first;
        }


        const Colour& ColourBlender::logic( unsigned int _deltaTicks )
        {
            if( currentTime < totalTime )
            {
                currentTime += _deltaTicks;
                if(currentTime >= totalTime)
                {
                    currentTime = totalTime;
                }


                float r = (float)currentTime/totalTime;
                if( forward )
                    current = Colour( lin(first.r, second.r, r), lin(first.g, second.g, r), lin(first.b, second.b, r), lin(first.a, second.a, r) );
                else
                    current = Colour( lin(second.r, first.r, r), lin(second.g, first.g, r), lin(second.b, first.b, r), lin(second.a, first.a, r) );

            }
            return current;
        }

        bool ColourBlender::finished()
        {
            return (currentTime >= totalTime);
        }


        void ColourBlender::reverse()
        {
            currentTime = totalTime - currentTime;
            forward = !forward;
        }
    }
}
