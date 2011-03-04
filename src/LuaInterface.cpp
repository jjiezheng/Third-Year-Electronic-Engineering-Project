#include "LuaInterface.h"

#include "luaInput.h"
#include "luaSystem.h"
#include "luaGraphics.h"
#include "luaCollision.h"



namespace de
{
    namespace luaInterface
    {
		void exportAPI( lua_State *_luaState )
		{
			export2D( _luaState );
			export3D( _luaState );
			exportLuaState( _luaState );
			exportCollision( _luaState );
			exportInput( _luaState );
			luaSystem( _luaState );
		}
    }
}
