#ifndef SOUNDEFFECTRESOURCE_H
#define SOUNDEFFECTRESOURCE_H

#include "Base.h"


namespace de
{
    namespace resources
    {
        class SoundEffectResource
        {
            public:
                //! Basic Constructor.
                SoundEffectResource() : SoundEffect(0) {}
                //! Constructor with FileName passed in.
                SoundEffectResource( const std::string &_name, const boost::filesystem::path &_path );
                ~SoundEffectResource();

                const int& getSoundEffect();
                void load();
                void unload();
            private:
                int SoundEffect;
                std::string name;
                boost::filesystem::path path, luaPath;
        };
	}
}
#endif //SOUNDEFFECTRESOURCE_H
