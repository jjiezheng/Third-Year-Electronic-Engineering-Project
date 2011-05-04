#include "Sound.h"
#include "CoreEnumsAndClasses.h"

#include <iostream>
#include <luabind/luabind.hpp>

#include <SDL/SDL_mixer.h>


using namespace de::internal;
using namespace de::filesystem;

namespace fs = boost::filesystem;

namespace de
{
    namespace internal
    {
        struct Music;
        struct SoundEffect;
        //! Music resource used internally by Sound.
        struct Music
        {
            //! Pointer to heap allocated Music
            Mix_Music* music;
            //! Unique handle to Music
            int handle;

            //! Basic Constructor.
            Music() : music(NULL), handle(0) {}

        };
        //! SoundEffect resource used internally by Sound.
        struct SoundEffect
        {
            //! Pointer to heap allocated soundEffect
            Mix_Chunk* soundEffect;
            //! Unique handle to SoundEffect
            int handle;

            //! Basic Constructor.
            SoundEffect() : soundEffect(NULL), handle(0) {}
        };
    }
    namespace core
    {
        SDLSound::SDLSound()
            :numberOfChannels(0), musicVolume(0), fxVolume(0)
        {
            luaState = lua_open();
            luaL_openlibs(luaState);
            luabind::open(luaState);

            fs::path dir( Roots->get( root::SETTINGS ) + "Sound.lua" );
            luaL_dofile( luaState, dir.file_string().c_str() );

            SDL_Init( SDL_INIT_AUDIO );
            de::io::log << "Starting SDL mixer... ";
            Mix_Init( MIX_INIT_OGG );
            if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 )
            {
                error = true;
                errorString = "Failure to start SDL mixer...";
                de::io::error << errorString << "\n";
            }
            de::io::log << "Done\n";


            loadSettings();
        }
        SDLSound::~SDLSound()
        {
			saveSettings();
            lua_close(luaState);
            de::io::log << "Closing SDL mixer... ";
            Mix_CloseAudio();
            de::io::log << "Done\n";
        }

		void SDLSound::loadSettings()
		{
			luabind::object global = luabind::globals(luaState);
			luabind::object SoundTable = global["Sound"];
			if( luabind::type(SoundTable) != LUA_TTABLE )
            {
                error = true;
                errorString = "Problem in the Sound config file. Can't get the sound table. Switching to sane presets...";
                de::io::error << errorString << "\n";
                numberOfChannels = 16;
				musicVolume = 5;
				fxVolume = 5;
            }
            else
			{
                numberOfChannels = luabind::object_cast<int>( SoundTable["channels"] );
				musicVolume = luabind::object_cast<int>( SoundTable["music_volume"] );
				fxVolume = luabind::object_cast<int>( SoundTable["fx_volume"] );

				//musicVolume = (MIX_MAX_VOLUME*musicVolume)/10;
				//fxVolume = (MIX_MAX_VOLUME*fxVolume)/10;
			}
			

			numberOfChannels = Mix_AllocateChannels( numberOfChannels );
			Mix_VolumeMusic( (MIX_MAX_VOLUME*musicVolume)/10 );

		}


		void SDLSound::saveSettings()
        {
            de::io::log << "Saving Sound settings\n";

            fs::path serialize( Roots->get( root::LUA ) + "serialize.lua" );
            fs::path savePath( Roots->get( root::SETTINGS ) + "Sound.lua" );

            luaL_dofile( luaState, serialize.file_string().c_str() );

            luabind::object global = luabind::globals(luaState);
            luabind::object SoundTable = global["Sound"];

            SoundTable["channels"] = numberOfChannels;
            SoundTable["music_volume"] = musicVolume;
            SoundTable["fx_volume"] = fxVolume;

            try
            {
                std::string serialisedTable = luabind::call_function<std::string>( global["serialize"], SoundTable );

                std::ofstream file;
                file.open( savePath.file_string().c_str() );
                file << "Sound = " << serialisedTable << "\n";
                file.close();
            }
            catch( luabind::error &e)
            {
                std::string error = lua_tostring( e.state(), -1 );
                de::io::error << e.what() << "\n";
                de::io::error << error << "\n";
            }
        }

        int SDLSound::loadMusic( const std::string &_filePath )
        {
            static int i(0);
            ++i;

            Music buffer;
            buffer.handle = i;
            buffer.music = Mix_LoadMUS( _filePath.c_str() );

            if( buffer.music == NULL )
            {
                de::io::error << "Failure to load music at |" << _filePath.c_str() << "|\n";
                return 0;
            }
            music[buffer.handle] = buffer;

            return i;
        }
        int SDLSound::loadSoundEffect( const std::string &_filePath )
        {
            //Mix_Volume( -1, MIX_MAX_VOLUME/48 );


            static int j(0);
            ++j;

            SoundEffect buffer;

            buffer.handle = j;

            buffer.soundEffect = Mix_LoadWAV( _filePath.c_str() );
            if( buffer.soundEffect == NULL )
            {
                de::io::error << "Failure to load sound effect at |" << _filePath.c_str() << "|\n";
                --j;
                return 0;
            }
			soundEffect[buffer.handle] = buffer;

            return j;
        }

        bool SDLSound::playMusic( int _handle )
        {
			de::io::tests << "playMusic : " << _handle  << "\n";
			iterMusic = music.find( _handle );
			if( iterMusic == music.end() )
            {
                de::io::error << "No Music to play\n";
                return false;
            }

           if( Mix_PlayMusic( iterMusic->second.music, -1 ) == -1 )
            {
				de::io::error << "Failure to play music\n";
                return false;
            }
			return true;
        }
        bool SDLSound::playSoundEffect( int _handle )
        {
			iterSoundEffect = soundEffect.find( _handle );
			if( iterSoundEffect == soundEffect.end() )
            {
				de::io::error << "No sound effect to play\n";
                return false;
            }

            if( Mix_PlayChannel( -1, iterSoundEffect->second.soundEffect, 0 ) == -1 )
            {
                numberOfChannels = Mix_AllocateChannels( numberOfChannels * 2 );
                if( Mix_PlayChannel( -1, iterSoundEffect->second.soundEffect, 0 ) == -1 )
                {
                    de::io::error << "Failure to play sound effect\n";
                    return false;
                }
			}
			Mix_VolumeChunk( iterSoundEffect->second.soundEffect, (MIX_MAX_VOLUME*fxVolume)/10 );
			return true;
        }

        bool SDLSound::deleteMusic( int _handle )
        {
			iterMusic = music.find( _handle );
			if( iterMusic == music.end() )
            {
                return false;
            }
			music.erase( iterMusic );
			return true;
        }
        bool SDLSound::deleteSoundEffect( int _handle )
        {
			iterSoundEffect = soundEffect.find( _handle );
			if( iterSoundEffect == soundEffect.end() )
            {
                return false;
            }
			soundEffect.erase( iterSoundEffect );
			return true;
        }

        void SDLSound::setMusicVolume( const float &_volumeLevel )
        {
            Mix_VolumeMusic( MIX_MAX_VOLUME*(int)_volumeLevel );
        }

        bool SDLSound::stopMusic()
        {
            Mix_HaltMusic();
            return true;
        }
        bool SDLSound::pauseMusic()
        {
            if( Mix_PausedMusic() == 1 )
            {
                return false;
            }
            else
            {
                Mix_PauseMusic();
                return true;
            }
        }
        bool SDLSound::resumeMusic()
        {
            if( Mix_PausedMusic() != 1 )
            {
                return false;
            }
            else
            {
                Mix_ResumeMusic();
                return true;
            }
        }

		void SDLSound::pushSettings( const std::string &_serialisedText )
		{
			//luaL_dostring( luaState, "Sound = nil" );
			luaL_dostring( luaState, _serialisedText.c_str() );
			loadSettings();
		}

        bool SDLSound::isOk()
        {
            //Do Stuff
            return !error;
        }

        std::string SDLSound::name()
        {
            static std::string _name("SDLSound");
            return _name;
        }
        std::string SDLSound::about()
        {
            static std::string info("Module:SDLSound, Type:IAudio, Info:Implments audio by using the SDL_audio library");
            return info;
        }
        std::string SDLSound::getError()
        {
            if( error )
                return errorString;
            else
                return "No Error.";
        }
    }
}
