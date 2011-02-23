#ifndef HITBOX_H
#define HITBOX_H

#include "CoreEnumsAndClasses.h"
#include "LinedBox.h"
#include "Vector.h"
#include "Line.h"


namespace de
{
    namespace coords
    {
        class Hitbox
        {
            public:
                Hitbox();
                virtual ~Hitbox();
            protected:
            private:
                de::enums::Collision label;
                de::classes::Poly hitbox;
                de::classes::LinedBox visualHitbox;
        };
    }
}


#endif // HITBOX_H
