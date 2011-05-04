#ifndef LOGGEDAUDIO_H
#define LOGGEDAUDIO_H

#include "IAudio.h"


class LoggedAudio : public IAudio
{
    public:
        LoggedAudio(IAudio *_wrapped);
        virtual ~LoggedAudio();

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
        //! Resume playing currently paused music
        bool resumeMusic();
        //! Stop currently playing music
        bool stopMusic();

		void pushSettings( const std::string &_serialisedText );

        bool isOk();
        std::string name();
        std::string about();
        std::string getError();
    protected:
    private:
        IAudio *wrapped;
};

#endif // LOGGEDAUDIO_H
