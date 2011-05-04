#include "LuaState.h"
//#include "Pixelstorm.h"
#include "CoreEngine.h"
#include <lua.hpp>

namespace fs = boost::filesystem;
using namespace de::filesystem;

namespace
{
    const std::string& pathToChunks()
    {
        static fs::path dir( Roots->get( root::LUA ) + "Chunks/" );
        static std::string chunks = dir.file_string();

        return chunks;
    }

    std::string include( const std::string &_fileName )
    {
        return pathToChunks() + _fileName + ".lua";
    }

    void print( const std::string &_message )
    {
        de::io::tests << _message << "\n";
    }

    void setResources( const std::string &_location )
	{
        de::Engine::Resources().load( _location );
    }

    void exportOther( lua_State *_luaState )
    {
        luabind::module(_luaState)
        [
            luabind::def( "include", (std::string(*)(const std::string &)) &::include ),
            luabind::def( "Log", (void(*)(const std::string &)) &::print ),
            luabind::def( "loadResources", (void(*)(const std::string &)) &::setResources )
        ];
    }
}


namespace de
{
	std::map< std::string, void (*)(lua_State *_luaState) > LuaAPI;

	void addLuaInterface( const std::string &_interfaceName, void (*_interface)(lua_State *) )
	{
		LuaAPI[_interfaceName] = _interface;
	}

	namespace state
	{
		LuaState::LuaState( const std::string &_luaPath, bool _altControls ) :luaError(false), active(false), altControls(_altControls), luaState(NULL)
		{
			ErrorInstructions.font( "visitor" ).shader( "String" ).depth( false );
			ErrorInstructions.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
			ErrorInstructions.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0), glm::vec3(0.0f,20.0f,-950.0f ) ), 180.0f, glm::vec3( 1.0f,0.0f,0.0f) ) );
			ErrorInstructions.setUniform( "Colour", glm::vec4( 0.67f, 0.16f, 0.16f,0.8f ) );
			ErrorInstructions.text( "Error in current lua file. Edit then press F5." );

			ErrorFile.font( "visitor" ).shader( "String" ).depth( false );
			ErrorFile.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
			ErrorFile.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0), glm::vec3(0.0f,-20.0f,-950.0f ) ), 180.0f, glm::vec3( 1.0f,0.0f,0.0f) ) );
			ErrorFile.setUniform( "Colour", glm::vec4( 0.67f, 0.16f, 0.16f,0.8f ) );
			ErrorFile.text( "Error in lua file. Edit then press F5." );

			message.font( "visitor" ).shader( "String" ).depth( false );
			message.setUniform( "Projection", glm::perspective( 45.0f, 16.0f/10.0f, 0.1f, 1000.0f ) );
			message.setUniform( "View", glm::rotate( glm::translate( glm::mat4(1.0), glm::vec3(0.0f,0.0f,-950.0f ) ), 180.0f, glm::vec3( 1.0f,0.0f,0.0f) ) );
			message.setUniform( "Colour", glm::vec4( 0.67f, 0.16f, 0.16f,0.8f ) );
			message.text( "Error in lua file. Edit then press F5." );

			luaPath = _luaPath;
			loadLuaState( luaPath );
		}

		LuaState::~LuaState()
		{
			if( luaState )
			{
				/*lua_close(luaState);
				luaState = NULL;*/
			}
		}

		void LuaState::loadLuaState( const std::string &_luaPath )
		{
			luaState = luaL_newstate();
			luaL_openlibs(luaState);
			luabind::open(luaState);
			luabind::set_pcall_callback( &add_file_and_line );

			for( iter = LuaAPI.begin(); iter != LuaAPI.end(); ++iter )
			{
				iter->second(luaState);
			}
			exportOther(luaState);


			fs::path dir( Roots->get( root::LUA ) + "States/" + _luaPath + ".lua" );
			if( luaL_dofile( luaState, dir.file_string().c_str() ) )
			{
				add_file_and_line(luaState);
				luaError = true;
				de::io::error << de::luaError::message;
				message.text( de::luaError::message );
				ErrorFile.text( "The file is at " + de::luaError::file );
			}
			else
			{
				luaError = false;
			}
			
			ErrorFile.text( "The file is at \"" + Roots->get( root::LUA ) + _luaPath + ".lua\"" );

			if( !luaError)
			{
				try
				{
					global = luabind::globals(luaState);
					currentState = global[_luaPath];
					luabind::call_function<void>( currentState["Start"], currentState );
				}
				catch( luabind::error &e)
				{
					luaError = true;
					de::io::error << de::luaError::message;
					message.text( de::luaError::message );
				}
			}

		}

		bool LuaState::handleEvents( const SDL_Event &_event )
		{
			if( !luaError)
			{
				try
				{
					if( luabind::call_function<bool>( currentState["HandleEvents"], currentState, _event ) )
					{
						loadLuaState( luaPath );
					}
				}
				catch( luabind::error &e)
				{
					luaError = true;
					de::io::error << de::luaError::message;
					message.text( de::luaError::message );
					ErrorFile.text( "The file is at " + de::luaError::file );
				}
			}
			if( altControls )
			{
				if( _event.type == SDL_KEYDOWN )
				{
					if( _event.key.keysym.sym == SDLK_F7 )
					{
						loadLuaState( luaPath );
					}
					else if( _event.key.keysym.sym == SDLK_F8 )
					{
						loadLuaState( "Bootstrap" );
					}
				}
			}
			else
			{
				if( _event.type == SDL_KEYDOWN )
				{
					if( _event.key.keysym.sym == SDLK_F5 )
					{
						loadLuaState( luaPath );
					}
					else if( _event.key.keysym.sym == SDLK_F6 )
					{
						loadLuaState( "Bootstrap" );
					}
				}
			}


			if( _event.type == SDL_USEREVENT )
			{
				if( _event.user.code == de::enums::events::OPENGL_RELOAD )
				{
					reLoadTextures();
				}
			}

			return true;
		}

		bool LuaState::logic( const float &_deltaTicks, State* &_nextState, state::options &_options )
		{
			return logic( _deltaTicks );
		}

		bool LuaState::logic( const float &_deltaTicks )
		{
			if( !luaError)
			{
				try
				{
					luabind::call_function<void>( currentState["Logic"], currentState, _deltaTicks );
				}
				catch( luabind::error &e)
				{
					luaError = true;
					de::io::error << de::luaError::message;
					message.text( de::luaError::message );
					ErrorFile.text( "The file is at " + de::luaError::file );
				}
			}
			return true;
		}

		void LuaState::render()
		{
			if( !luaError)
			{
				try
				{
					luabind::call_function<void>( currentState["Render"], currentState );
				}
				catch( luabind::error &e)
				{
					luaError = true;
					de::io::error << de::luaError::message;
					message.text( de::luaError::message );
					ErrorFile.text( "The file is at " + de::luaError::file );
				}
			}
			else
			{
				ErrorInstructions.render();
				ErrorFile.render();
				message.render();
			}
		}

		void LuaState::reLoadTextures()
		{
			if( !luaError)
			{
				try
				{
					luabind::call_function<void>( currentState["Reload"], currentState );
				}
				catch( luabind::error &e)
				{
					luaError = true;
					de::io::error << de::luaError::message;
					message.text( de::luaError::message );
					ErrorFile.text( "The file is at " + de::luaError::file );
				}
			}

			message.reload();
			ErrorInstructions.reload();
			ErrorFile.reload();
		}
	}
}
