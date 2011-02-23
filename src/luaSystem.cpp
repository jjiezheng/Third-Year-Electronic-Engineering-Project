#include "luaSystem.h"


namespace de
{
    namespace luaInterface
    {
        void luaSystem( lua_State *_luaState )
		{
			luabind::module(_luaState, "sys" )
            [
                luabind::def( "getTime",         (std::string(*)()) &::de::time::getTimeString ),
                luabind::def( "getFrameRate",    (std::string(*)()) &::de::sys::getFrameRate ),
                luabind::def( "getspriteNumber", (std::string(*)()) &::de::sys::getspriteNumber ),
                luabind::def( "getbatchNumber",  (std::string(*)()) &::de::sys::getbatchNumber ),
                luabind::def( "getResolution",   (std::string(*)()) &::de::sys::getResolution ),
                luabind::def( "title",           (void(*)(const std::string &)) &::de::sys::titleBar )
            ];
		}
    }
}
