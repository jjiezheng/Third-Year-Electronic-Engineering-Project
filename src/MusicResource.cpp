#include "MusicResource.h"
#include "GameServices.h"

namespace de
{
    namespace resources
    {
        MusicResource::MusicResource( const std::string &_name, const boost::filesystem::path &_path ) : Music(0)
        {
            name = _name;
            path = _path;
        }

        MusicResource::~MusicResource()
        {
            unload();
        }

        const int& MusicResource::getMusic()
        {
            if( Music == 0 )
            {
                load();
            }
            return Music;
        }

        void MusicResource::load()
        {
            if( Music == 0 )
            {
                Music = Engine::Audio().loadMusic( path.file_string() );
            }
        }

        void MusicResource::unload()
        {
            Engine::Audio().deleteMusic( Music );
            Music = 0;
        }
	}
}