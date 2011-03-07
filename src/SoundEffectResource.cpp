#include "SoundEffectResource.h"
#include "GameServices.h"

namespace de
{
    namespace resources
    {
        SoundEffectResource::SoundEffectResource( const std::string &_name, const boost::filesystem::path &_path ) : SoundEffect(0)
        {
            name = _name;
            path = _path;
        }

        SoundEffectResource::~SoundEffectResource()
        {
            unload();
        }

        const int& SoundEffectResource::getSoundEffect()
        {
            if( SoundEffect == 0 )
            {
                load();
            }
            return SoundEffect;
        }

        void SoundEffectResource::load()
        {
            if( SoundEffect == 0 )
            {
                SoundEffect = Engine::Audio().loadSoundEffect( path.file_string() );
            }
        }

        void SoundEffectResource::unload()
        {
            Engine::Audio().deleteSoundEffect( SoundEffect );
            SoundEffect = 0;
        }
	}
}