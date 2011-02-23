#include "LuaState.h"
#include "Pixelstorm.h"
#include "CoreEngine.h"

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

    float get( const de::classes::Vector &_vec, bool _x )
    {
        if( _x )
            return _vec.x;
        return _vec.y;
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
            luabind::def( "get", (float(*)(const de::classes::Vector &, bool)) &::get ),
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
			ErrorInstructions.set( "Error in lua file. Edit then press F5.", "GeoSansLight", de::classes::Vector(640, 380), 11, de::enums::ALIGN_CENTRE );
			ErrorInstructions.setColour( de::classes::Colour( 1.0, 0.0, 0.0, 1.0 ) );

			ErrorFile.set( "", "GeoSansLight", de::classes::Vector(640, 420), 11, de::enums::ALIGN_CENTRE );
			ErrorFile.setColour( de::classes::Colour( 1.0, 0.0, 0.0, 1.0 ) );

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

		for( iter = LuaAPI.begin(); iter != LuaAPI.end(); ++iter )
		{
			iter->second(luaState);
		}
        exportOther(luaState);


        fs::path dir( Roots->get( root::LUA ) + "States/" + _luaPath + ".lua" );
        if( luaL_dofile( luaState, dir.file_string().c_str() ) )
        {
            luaError = true;
            message = "Lua Sytax Error";
        }
        else
        {
            luaError = false;
        }

        ErrorFile = "The file is \"" + Roots->get( root::LUA ) + _luaPath + ".lua" + "\"";
        fade.Setup( de::classes::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) , de::classes::Colour( 0.0f, 0.0f, 0.0f, 0.0f ) , 3000 );

        if( altControls )
        {
            message.set( "State " + _luaPath +" Loaded", "System", de::classes::Vector(2, 20), 11, de::enums::ALIGN_RIGHT );
        }
        else
        {
            message.set( "State " + _luaPath +" Loaded", "System", de::classes::Vector(2, 10), 11, de::enums::ALIGN_RIGHT );
        }

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
                std::string error = lua_tostring( e.state(), -1 );

                de::io::error << e.what() << "\n";
                de::io::error << error << "\n";

                luaError = true;
                message = "Lua Error :Start";
                fade.Setup( de::classes::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) , de::classes::Colour( 0.0f, 0.0f, 0.0f, 0.0f ) , 3000 );
            }
        }

    }

		bool LuaState::handleEvents( const SDL_Event &_event )
		{
			if( _event.type == SDL_USEREVENT )
			{
				if( _event.user.code == de::enums::events::OPENGL_RELOAD )
				{
					reLoadTextures();
				}
			}

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
					std::string error = lua_tostring( e.state(), -1 );
					de::io::error << e.what() << "\n";
					de::io::error << error << "\n";

					luaError = true;
					message = "Lua Error :HandleEvents";
					fade.Setup( de::classes::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) , de::classes::Colour( 0.0f, 0.0f, 0.0f, 0.0f ) , 3000 );
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

			return true;
		}

		bool LuaState::logic( const Uint32 &_deltaTicks, State* &_nextState, state::options &_options )
		{
			return logic( _deltaTicks );
		}

		bool LuaState::logic( const Uint32 &_deltaTicks )
		{
			if( !luaError)
			{
				try
				{
					luabind::call_function<void>( currentState["Logic"], currentState, (int)_deltaTicks );
				}
				catch( luabind::error &e)
				{
					std::string error = lua_tostring( e.state(), -1 );
					de::io::error << e.what() << "\n";
					de::io::error << error << "\n";

					luaError = true;
					message = "Lua Error :Logic";
					fade.Setup( de::classes::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) , de::classes::Colour( 0.0f, 0.0f, 0.0f, 0.0f ) , 3000 );
				}
			}

        message.setColour( fade.logic( _deltaTicks ) );

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
					std::string error = lua_tostring( e.state(), -1 );
					de::io::error << e.what() << "\n";
					de::io::error << error << "\n";

					luaError = true;
					message = "Lua Error :Render";
					fade.Setup( de::classes::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) , de::classes::Colour( 0.0f, 0.0f, 0.0f, 0.0f ) , 3000 );
				}
			}
			else
			{
				ErrorInstructions.render( de::enums::FBO_AFTER );
				ErrorFile.render( de::enums::FBO_AFTER );
			}

			message.render( de::enums::FBO_AFTER );
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
					std::string error = lua_tostring( e.state(), -1 );
					de::io::error << e.what() << "\n";
					de::io::error << error << "\n";

					luaError = true;
					message = "Lua Error :Reload";
					fade.Setup( de::classes::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) , de::classes::Colour( 0.0f, 0.0f, 0.0f, 0.0f ) , 3000 );
				}
			}
			message.reload();
			ErrorInstructions.reload();
			ErrorFile.reload();
		}
	}
}
