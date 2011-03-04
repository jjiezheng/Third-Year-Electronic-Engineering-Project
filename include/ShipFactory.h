#ifndef SHIPFACTORY_H
#define SHIPFACTORY_H

#include "CoreEngine.h"
#include "Coords.h"
#include "Ships.h"

#include <luabind/luabind.hpp>
#include <lua.hpp>

namespace Attrition
{
    class ShipFactory
    {
        public:
            ShipFactory();
            virtual ~ShipFactory();

            void parseShips();
            const Ship& get( const std::string &_shipName );
            const std::vector<std::string>& shipNames();


        private:
            std::map<std::string, Ship> Ships;
            std::map<std::string, Ship>::iterator shipsIter;
            typedef std::pair<std::string, Ship> shipType;

            std::vector<std::string> ShipNames;

            bool parsingDone;
    };

    extern ShipFactory shipManager;
}


#endif // SHIPFACTORY_H
