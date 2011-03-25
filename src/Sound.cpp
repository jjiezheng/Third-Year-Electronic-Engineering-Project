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
        SDLSound* Sound = NULL;



        SDLSound::SDLSound()
            :numberOfChannels(0)
        {
            luaState = lua_open();
            luaL_openlibs(luaState);
            luabind::open(luaState);

            fs::path dir( Roots->get( root::SETTINGS ) + "Sound.lua" );

            luaL_dofile( luaState, dir.file_string().c_str() );

            luabind::object global = luabind::globals(luaState);
            luabind::object SoundTable = global["Sound"];

            if( luabind::type(SoundTable) != LUA_TTABLE )
            {
                error = true;
                errorString = "Problem in the Sound config file. Can't get the sound table. Switching to sane presets...";
                de::io::error << errorString << "\n";
                numberOfChannels = 16;
            }
            else
                numberOfChannels = luabind::object_cast<int>( SoundTable["channels"] );


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

            numberOfChannels = Mix_AllocateChannels( numberOfChannels );
        }

        SDLSound::~SDLSound()
        {
            lua_close(luaState);
            de::io::log << "Closing SDL mixer... ";
            Mix_CloseAudio();
            de::io::log << "Done\n";
        }


        //Returns a int that stands for the music Loaded in this function
        int SDLSound::loadMusic( const std::string &_filePath )
        {
            Mix_VolumeMusic(MIX_MAX_VOLUME);

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
            musicVector.push_back( buffer );

            return i;
        }

        int SDLSound::loadSoundEffect( const std::string &_filePath )
        {
            Mix_Volume( -1, MIX_MAX_VOLUME/48 );


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
            soundEffectVector.push_back( buffer );

            return j;
        }

        bool SDLSound::playMusic( int _handle )
        {
            for( iterMusic = musicVector.begin(); iterMusic < musicVector.end(); ++iterMusic )
            {
                if( iterMusic->handle == _handle )
                {
                    //Play the music
                    if( Mix_PlayMusic( iterMusic->music, -1 ) == -1 )
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            de::io::error << "No Music to play\n";
            return false;
        }

        bool SDLSound::playSoundEffect( int _handle )
        {
            for( iterSoundEffect = soundEffectVector.begin(); iterSoundEffect < soundEffectVector.end(); ++iterSoundEffect )
            {
                if( iterSoundEffect->handle == _handle )
                {
                    //Play the music
                    if( Mix_PlayChannel( -1, iterSoundEffect->soundEffect, 0 ) == -1 )
                    {
                        numberOfChannels = Mix_AllocateChannels( numberOfChannels * 2 );

                        if( Mix_PlayChannel( -1, iterSoundEffect->soundEffect, 0 ) == -1 )
                        {
                            de::io::error << "Failure to play sound effect\n";
                            return false;
                        }
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            de::io::error << "No Sound Effect to play\n";
            return true;
        }

        bool SDLSound::deleteMusic( int _handle )
        {
            for( iterMusic = musicVector.begin(); iterMusic < musicVector.end(); ++iterMusic )
            {
                if( iterMusic->handle == _handle )
                {
                    Mix_FreeMusic( iterMusic->music );
                    iterMusic = musicVector.erase( iterMusic );
                    return true;
                }
            }
            return false;
        }

        bool SDLSound::deleteSoundEffect( int _handle )
        {
            for( iterSoundEffect = soundEffectVector.begin(); iterSoundEffect < soundEffectVector.end(); ++iterSoundEffect )
            {
                if( iterSoundEffect->handle == _handle )
                {
                    Mix_FreeChunk( iterSoundEffect->soundEffect );
                    iterSoundEffect = soundEffectVector.erase( iterSoundEffect );
                    return true;
                }
            }
            return false;
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
