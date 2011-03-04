#include "LoggedAudio.h"
#include "Log.h"


LoggedAudio::LoggedAudio(IAudio *_wrapped) : wrapped(_wrapped)
{

}

LoggedAudio::~LoggedAudio()
{
    delete wrapped;
}

int LoggedAudio::loadMusic( const std::string &_filePath  )
{
    de::io::log << "loadMusic called with _filePath = " << _filePath << "\n";
    return wrapped->loadMusic( _filePath );
}

int LoggedAudio::loadSoundEffect( const std::string &_filePath )
{
    de::io::log << "loadSoundEffect called with _filePath = " << _filePath << "\n";
    return wrapped->loadSoundEffect( _filePath );
}

bool LoggedAudio::playMusic( int _handle )
{
    de::io::log << "playMusic called with _handle = " << _handle << "\n";
    return wrapped->playMusic( _handle );
}

bool LoggedAudio::playSoundEffect( int _handle )
{
    de::io::log << "playSoundEffect called with _handle = " << _handle << "\n";
    return wrapped->playSoundEffect( _handle );
}

bool LoggedAudio::deleteMusic( int _handle )
{
    de::io::log << "deleteMusic called with _handle = " << _handle << "\n";
    return wrapped->deleteMusic( _handle );
}

bool LoggedAudio::deleteSoundEffect( int _handle )
{
    de::io::log << "deleteSoundEffect called with _handle = " << _handle << "\n";
    return wrapped->deleteSoundEffect( _handle );
}

void LoggedAudio::setMusicVolume( const float &_volumeLevel )
{
    de::io::log << "setMusicVolume called with _volumeLevel = " << _volumeLevel << "\n";
    wrapped->setMusicVolume( _volumeLevel );
}

bool LoggedAudio::pauseMusic()
{
    de::io::log << "pauseMusic called.\n";
    return wrapped->pauseMusic();
}

bool LoggedAudio::resumeMusic()
{
    de::io::log << "resumeMusic called.\n";
    return wrapped->resumeMusic();
}

bool LoggedAudio::stopMusic()
{
    de::io::log << "stopMusic called.\n";
    return wrapped->stopMusic();
}


bool LoggedAudio::isOk()
{
    de::io::log << "isOk called.\n";
    return wrapped->isOk();
}

std::string LoggedAudio::name()
{
    static std::string _name("LoggedAudio");
    return _name;
}

std::string LoggedAudio::about()
{
    de::io::log << "about called.\n";
    return wrapped->about();
}

std::string LoggedAudio::getError()
{
    de::io::log << "getError called.\n";
    return wrapped->getError();
}

