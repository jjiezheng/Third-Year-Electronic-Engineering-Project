#include "LuaInterface.h"

#include "luaInput.h"
#include "luaSystem.h"
#include "luaGraphics.h"
#include "luaCollision.h"
#include "luaHaptics.h"
#include "luaSound.h"
#include "luaMatrix.h"
#include "luaSplines.h"

namespace de
{
    namespace luaInterface
    {
		void exportAPI( lua_State *_luaState )
		{
			export3D( _luaState );
			exportLuaState( _luaState );
			exportCollision( _luaState );
			exportInput( _luaState );
			exportSplines( _luaState );
			exportMatrix( _luaState );
			luaSystem( _luaState );
			exportHaptics( _luaState );
			luaSound( _luaState );
		}
    }
}
