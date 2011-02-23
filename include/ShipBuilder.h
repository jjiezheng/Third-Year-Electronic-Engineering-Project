#ifndef SHIPBUILDER_H
#define SHIPBUILDER_H

#include "Coords.h"
#include "CoreEngine.h"
#include "Entities.h"
#include "UIbaseData.h"
#include "UIShipInfo.h"
#include "UIGunInfo.h"
#include "UILevelInfo.h"
#include "UIspline.h"


namespace Attrition
{
    namespace builder
    {
        class ShipBuilder
        {
            public:
                ShipBuilder();
                virtual ~ShipBuilder();

                void handleEvent( const SDL_Event &_event );
                void logic( const Uint32 &deltaTicks,
                            boost::ptr_vector<Attrition::Ship> &_Ships,
                            boost::ptr_vector<Bullet> &_Bullets );
                void reLoadTextures();
                void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );
            protected:
            private:
        };
    }
}
#endif // SHIPBUILDER_H
