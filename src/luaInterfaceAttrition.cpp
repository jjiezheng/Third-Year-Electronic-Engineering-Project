#include "luaInterfaceAttrition.h"

namespace Attrition
{
    namespace luaInterface
    {
        void exportShips( lua_State *_luaState )
        {
			/*
            luabind::module(_luaState, "Att" )
            [
                luabind::class_<ShipFactory>( "ShipFactory" )
                    .def( luabind::constructor<>() )
                    .def( "parse", (void(ShipFactory::*)()) &ShipFactory::parseShips )
                    .def( "get",   (const Ship&(ShipFactory::*)(const std::string &)) &ShipFactory::get ),

                luabind::class_<Ship>( "Ship" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor<const Ship &>() )
                    .def( "set", (void(Ship::*)(const de::classes::Vector&)) &Ship::setPosition )
                    .def( "render", (void(Ship::*)(const int&, bool)) &Ship::render )
                    .def( "reload", (void(Ship::*)()) &Ship::reLoad )
                    .def( "id", (int(Ship::*)()) &Ship::getId )
                           /*
            const Ship& get( const std::string &_shipName );
            const std::vector<std::string>& shipNames();
            ];*/
        }
    }
}
