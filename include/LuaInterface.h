#ifndef LUAINTERFACE_H
#define LUAINTERFACE_H

#include "CoreEngine.h"
#include "LuaInterfaceState.h"

namespace de
{
    namespace luaInterface
    {
        void exportAPI( lua_State *_luaState );
    }
}




#endif // LUAINTERFACE_H
