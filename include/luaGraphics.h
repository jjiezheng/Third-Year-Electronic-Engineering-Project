#ifndef LUAGRAPHICS_H
#define LUAGRAPHICS_H

#include "CoreEngine.h"
namespace de
{
    namespace luaInterface
    {
        void export2D( lua_State *_luaState );
        void export3D( lua_State *_luaState );
    }
}


#endif //LUAGRAPHICS_H