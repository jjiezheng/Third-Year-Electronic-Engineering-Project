#include "LuaInterfaceState.h"
#include "LuaState.h"
#include "Pixelstorm.h"

namespace
{
    void nextState( const std::string &_state )
    {
        Attrition::Game->setNextState( new de::state::LuaState( _state ) );
    }

    void load( const std::string &_state )
    {
        Attrition::Game->setNextState( new de::state::LuaState( _state ) );
    }
}
namespace de
{
    namespace luaInterface
    {
        void exportLuaState( lua_State *_luaState )
        {
            luabind::module(_luaState)
            [
                luabind::class_<de::state::LuaState>( "State" )
                    .def( luabind::constructor<const std::string &>() )
                    .def( "handleEvents", (bool(de::state::LuaState::*)(const SDL_Event &))         &de::state::LuaState::handleEvents )
                    .def( "logic",        (bool(de::state::LuaState::*)(const float &_deltaTicks)) &de::state::LuaState::logic )
                    .def( "render",       (void(de::state::LuaState::*)())                          &de::state::LuaState::render ),

                luabind::def( "printMessage", (void(*)(const std::string &)) &::de::io::logMessage ),
                luabind::def( "setState", (void(*)(const std::string &)) &::nextState )
            ];
        }
    }
}
