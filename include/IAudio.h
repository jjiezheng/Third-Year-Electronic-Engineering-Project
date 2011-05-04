#ifndef IAUDIO_H
#define IAUDIO_H

#include <string>

class IAudio
{
    public:

        IAudio() {}
        virtual ~IAudio() {}

        //! Loads the Music from the filepath "_filePath"
        virtual int loadMusic( const std::string &_filePath  ) = 0;
        //! Loads the SoundEffect from the filepath "_filePath"
        virtual int loadSoundEffect( const std::string &_filePath ) = 0;

        //! Play the Music with the handle "_handle"
        virtual bool playMusic( int _handle ) = 0;
        //! Play the SoundEffect with the handle "_handle"
        virtual bool playSoundEffect( int _handle ) = 0;

        //! Delete the Music with the handle "_handle"
        virtual bool deleteMusic( int _handle ) = 0;
        //! Delete the SoundEffect with the handle "_handle"
        virtual bool deleteSoundEffect( int _handle ) = 0;

        //! Sets the volume level of the Music playing. From 0.0 to 1.0
        virtual void setMusicVolume( const float &_volumeLevel ) = 0;
        //! Pause currently playing music
        virtual bool pauseMusic() = 0;
        //! Resume playing currently paused music
        virtual bool resumeMusic() = 0;
        //! Stop currently playing music
        virtual bool stopMusic() = 0;

		virtual void pushSettings( const std::string &_serialisedText ) = 0;

        virtual bool isOk() = 0;
        virtual std::string name() = 0;
        virtual std::string about() = 0;
        virtual std::string getError() = 0;

    protected:
    private:
};


class NullAudio : public IAudio
{
    public:
        NullAudio() {}
        virtual ~NullAudio() {}
        //! Loads the Music from the filepath "_filePath"
        int loadMusic( const std::string &_filePath  )  { return -1; }
        //! Loads the SoundEffect from the filepath "_filePath"
        int loadSoundEffect( const std::string &_filePath )  { return -1; }

        //! Play the Music with the handle "_handle"
        bool playMusic( int _handle )  { return true; }
        //! Play the SoundEffect with the handle "_handle"
        bool playSoundEffect( int _handle ) { return true; }

        //! Delete the Music with the handle "_handle"
        bool deleteMusic( int _handle ) { return true; }
        //! Delete the SoundEffect with the handle "_handle"
        bool deleteSoundEffect( int _handle ) { return true; }

        //! Sets the volume level of the Music playing. From 0.0 to 1.0
        void setMusicVolume( const float &_volumeLevel ) {}
        //! Pause currently playing music
        bool pauseMusic() { return true; }
        //! Resume playing currently paused music
        bool resumeMusic() { return true; }
        //! Stop currently playing music
        bool stopMusic() { return true; }

		void pushSettings( const std::string &_serialisedText ) {};

        bool isOk() { return true; }
        std::string name()
        {
            static std::string _name("NullAudio");
            return _name;
        }

        std::string about()
        {
            static std::string info("Module:NullAudio, Type:IAudio, Info:A blank class used when no IAudio module is set");
            return info;
        }

        std::string getError() { return "No errors."; }

    protected:
    private:
};



#endif // IAUDIO_H
