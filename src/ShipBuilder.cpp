#include "ShipBuilder.h"

using namespace de::classes;
using namespace de::enums;
using namespace de::theme;
using namespace de::misc;
using namespace de::core;

namespace Attrition
{
    namespace builder
    {
        ShipBuilder::ShipBuilder()
        {
            //ctor
        }

        ShipBuilder::~ShipBuilder()
        {
            //dtor
        }



        void ShipBuilder::handleEvent( const SDL_Event &_event )
        {

        }

        void ShipBuilder::logic( const Uint32 &deltaTicks,
                    boost::ptr_vector<Attrition::Ship> &_Ships,
                    boost::ptr_vector<Bullet> &_Bullets )
        {

        }

        void ShipBuilder::reLoadTextures()
        {

        }

        void ShipBuilder::render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 )
        {

        }
    }
}
