#ifndef LINED_BOX_H
#define LINED_BOX_H

#include "CoreEnumsAndClasses.h"
#include "Vector.h"
#include "Line.h"

namespace de
{
    namespace classes
    {
        //! A Lined Box. Uses both Poly and Line.
        /*! This class is used for rendering hitboxes on screen. Has different Settings depending on if the hitbox is enums::ENEMY or enums::FRIENDLY.
        */
        class LinedBox
        {
            public:
                //! Constructor
                LinedBox();
                //! Constructor
                LinedBox( const Poly &_newPosition );
                //! Destructor
                ~LinedBox() {};

                Poly& getPoly();

                //! LinedBox becomes the Poly "_newPosition".
                void update( const Poly &_newPosition );
                //! Render the LinedBox. Is a different colour if the hitbox is enums::ENEMY or enums::FRIENDLY.
                void render( const enums::Collision &_label, const int &_type );
                //! Render the LinedBox with the Colour "_colour"
                void render( const Colour &_colour, const int &_type );


            private:
                Poly box;
                Colour friendly, enemy;
                Line edge;
        };
    }
}



#endif //LINED_BOX_H


