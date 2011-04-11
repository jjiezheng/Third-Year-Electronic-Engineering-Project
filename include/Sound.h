#ifndef SOUND_H
#define SOUND_H

#include "IAudio.h"
#include "Monitor.h"
#include "Log.h"
#include "Base.h"



namespace de
{
    namespace internal
    {
        struct Music;
        struct SoundEffect;
    }

    namespace core
    {
        //! Low Level api for loading and playing music.
        /*! Designed to give OpenGL style opeque handles while handling actual memory allocation and de-allocation itself.
        */
        class SDLSound : public IAudio
        {
            public:
                SDLSound();
                virtual ~SDLSound();

                //! Loads the Music from the filepath "_filePath"
                int loadMusic( const std::string &_filePath  );
                //! Loads the SoundEffect from the filepath "_filePath"
                int loadSoundEffect( const std::string &_filePath );

                //! Play the Music with the handle "_handle"
                bool playMusic( int _handle );
                //! Play the SoundEffect with the handle "_handle"
                bool playSoundEffect( int _handle );

                //! Delete the Music with the handle "_handle"
                bool deleteMusic( int _handle );
                //! Delete the SoundEffect with the handle "_handle"
                bool deleteSoundEffect( int _handle );

                //! Sets the volume level of the Music playing. From 0.0 to 1.0
                void setMusicVolume( const float &_volumeLevel );
                //! Pause currently playing music
                bool pauseMusic();
                //! Resume currently paused music
                bool resumeMusic();
                //! Stop currently playing music
                bool stopMusic();

				void pushSettings( const std::string &_serialisedText );

                bool isOk();
                std::string name();
                std::string about();
                std::string getError();

            private:
				void loadSettings();
				void saveSettings();
                bool error;
                unsigned int numberOfChannels, musicVolume, fxVolume;
                std::string errorString;
                lua_State *luaState;


                std::map<int, de::internal::Music> music;
                std::map<int, de::internal::Music>::iterator iterMusic;

                std::map<int, de::internal::SoundEffect> soundEffect;
                std::map<int, de::internal::SoundEffect>::iterator iterSoundEffect;
        };
    }
}

#endif //SOUND_H
