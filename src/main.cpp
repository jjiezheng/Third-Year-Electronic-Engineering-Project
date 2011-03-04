/*
    Attrition -  2009-2011
    Copyright: Daniel Hartnett
    Email about license - thelinked@gmail.com
*/
#include "Pixelstorm.h"

#include "LuaState.h"
#include "AttritionTestState.h"
#include "TestState.h"

#include "LuaInterface.h"
#include "luaInterfaceAttrition.h"

int main( int argc, char* args[] )
{
	de::addLuaInterface( "API", &de::luaInterface::exportAPI );
	de::addLuaInterface( "Ships", &Attrition::luaInterface::exportShips );

    Attrition::Game = new Attrition::DifferenceEngine;
    Attrition::Game->setNextState( new de::state::LuaState( "Bootstrap" ) );
	//Attrition::Game->setNextState( new TestState() );
	Attrition::Game->changeState();

    //Attrition::Game->setNextState( new Attrition::AttritionTestState() );
    
	
    try
    {
        while ( Attrition::Game->running() )
            Attrition::Game->runCurrentState();
    }
    catch( std::exception &e )
    {
         de::io::log << e.what();
         assert(!"Uncaught exception. :(");
         delete Attrition::Game;
         return 1;
    }

    delete Attrition::Game;
    return 0;
}
