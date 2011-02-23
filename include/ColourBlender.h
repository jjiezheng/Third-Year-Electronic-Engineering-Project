#ifndef COLOURBLENDER_H
#define COLOURBLENDER_H

#include "CoreEnumsAndClasses.h"
#include "Vector.h"


namespace de
{
    namespace classes
    {
        //! Blends a Colour from One colour to another.
        class ColourBlender
        {
            public:
                //! Basic Constructor
                ColourBlender();
                //! Constructor
                ColourBlender( const Colour &_from, const Colour &_to, unsigned int _time );
                //! Basic Destructor
                virtual ~ColourBlender();

                //! Set the the Colours to blend from and to and the time to do it in.
                void Setup( const Colour &_from, const Colour &_to, unsigned int _time );
                //! Add _deltaTicks to the total time.
                const Colour& logic( unsigned int _deltaTicks );
                //! Returns true if blending is finished
                bool finished();
                //! Start blending in the reverse direction
                void reverse();

            private:
                bool forward;
                unsigned int totalTime, currentTime;
                Colour first, second, current;
        };
    }
}

#endif // COLOURBLENDER_H
