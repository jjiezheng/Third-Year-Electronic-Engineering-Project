#ifndef LUASTATE_H
#define LUASTATE_H

#include <State.h>
#include <string>
#include "StringObject.h"

namespace de
{

	extern std::map< std::string, void (*)(lua_State *_luaState) > LuaAPI;
	void addLuaInterface( const std::string &_interfaceName, void (*_interface)(lua_State *) );
	namespace state
	{
		class LuaState : public State
		{
			public:
				/** Default constructor */
				LuaState( const std::string &_luaPath, bool _altControls = false );
				/** Default destructor */
				virtual ~LuaState();

				bool handleEvents( const SDL_Event &_event );
				bool logic( const float &_deltaTicks, State* &_nextState, state::options &_options );
				bool logic( const float &_deltaTicks );
				void render();

			private:
				std::map< std::string, void (*)(lua_State *_luaState) >::iterator iter;
				void reLoadTextures();
				void loadLuaState( const std::string &_luaPath );

				bool luaError, active, stateActive, altControls;
				std::string luaPath;
				lua_State *luaState;
				luabind::object global, currentState;

				StringObject message, ErrorInstructions, ErrorFile;
				/*
				de::classes::SpriteString message, ErrorInstructions, ErrorFile;
				de::classes::ColourBlender fade;*/

		};
	}
}

#endif // LUASTATE_H
