#ifndef LUAINPUT_H
#define LUAINPUT_H

#include "CoreEngine.h"
namespace de
{
    namespace luaInterface
    {
        void exportInput( lua_State *_luaState );
        void exportKeyBoard( lua_State *_luaState );
		void exportMouse( lua_State *_luaState );
        void exportSDLKeys( lua_State *_luaState );
    }
}


#endif //LUAINPUT_H