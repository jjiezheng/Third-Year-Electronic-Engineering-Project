#include "luaSystem.h"
#include "CoreEnumsAndClasses.h"
#include "GameServices.h"
#include "Monitor.h"

namespace de
{
    namespace sys
    {
		void exit()
		{
			SDL_Event event;
            event.type = SDL_QUIT;
            SDL_PushEvent(&event);
		}

		bool saveLuaText( const std::string &_text, const std::string &_fileName )
		{
			using namespace de::filesystem;

			try
			{
                std::ofstream file;
                file.open( Roots->get( root::SETTINGS ) + _fileName + ".lua" );
                file << _fileName << " = " << _text << "\n";
                file.close();
				de::io::log << "Saving to file: " << Roots->get( root::SETTINGS ) + _fileName + ".lua"  << "\n";
            }
            catch( luabind::error &e)
            {
                std::string error = lua_tostring( e.state(), -1 );
                de::io::error << e.what() << "\n";
                de::io::error << error << "\n";
				return false;
            }
			return true;
		}

		std::string loadLuaTextPath( const std::string &_file )
		{
			using namespace de::filesystem;
			boost::filesystem::path dir( Roots->get( root::SETTINGS ) + _file + ".lua" );
			de::io::tests << "Loading " << dir.file_string() << "\n";
			std::string chunks = dir.file_string();

			return chunks;
		}

		bool push_settings( const std::string &_serialisedText, const std::string &_system )
		{
			if( _system == "Sound" )
			{
				Engine::Audio().pushSettings( _serialisedText );
			}
			else if( _system == "Graphics" )
			{
				Engine::Graphics().pushSettings( _serialisedText );
			}
			return false;
		}

		int getRes( const std::string &_dem )
		{
			de::graphics::VideoInfo info = Engine::Graphics().getVideoSettings();

			if( _dem == "width" )
			{
				if( info.fullScreen )
					return info.fullscreenWidth;
				return info.screenWidth;
			}
			else if( _dem == "height" )
				if( info.fullScreen )
					return info.fullscreenHeight;
				return info.screenHeight;

			return 0;
		}
	}
}
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
                luabind::def( "getResolution",   (std::string(*)()) &::de::sys::getResolution ),
				luabind::def( "get",			 (int(*)(const std::string &)) &::de::sys::getRes ),
                luabind::def( "title",           (void(*)(const std::string &)) &::de::sys::titleBar ),
				luabind::def( "exit",            (void(*)()) &::de::sys::exit ),
				luabind::def( "save_setting",    (bool(*)(const std::string &,const std::string &)) &::de::sys::saveLuaText ),
				luabind::def( "load_settings",   (std::string(*)(const std::string &)) &::de::sys::loadLuaTextPath ),
				luabind::def( "push_settings",   (bool(*)(const std::string &, const std::string &)) &::de::sys::push_settings )
            ];
		}
    }
}
