#include "ShipFactory.h"

using namespace de::filesystem;

namespace fs = boost::filesystem;


namespace Attrition
{
    ShipFactory shipManager;




    ShipFactory::ShipFactory() : parsingDone(false)
    {
    }

    ShipFactory::~ShipFactory()
    {
        //dtor
    }

    const Ship& ShipFactory::get( const std::string &_shipName )
    {
        shipsIter = Ships.find( _shipName );

        if( shipsIter != Ships.end() )
        {
            return shipsIter->second;
        }
        static Ship blank;

        return blank;
    }

    const std::vector<std::string>& ShipFactory::shipNames()
    {
        return ShipNames;
    }


    void ShipFactory::parseShips()
    {
        if( !parsingDone )
        {
            std::vector<fs::path> foundFiles;
            std::vector<fs::path>::iterator iter, end;

            fs::path shipFilePath( Roots->get( root::SHIPS ) );
            getFilesFrom( shipFilePath, foundFiles, filetypes::LUA );

            for( iter = foundFiles.begin(); iter < foundFiles.end(); ++iter )
            {
                std::string shipName( stripFileEnding( iter->filename() ) );

                Ships.insert(  shipType( shipName, Ship( shipName ) )  );
                ShipNames.push_back( shipName );
            }

            parsingDone = true;
        }
    }
}
