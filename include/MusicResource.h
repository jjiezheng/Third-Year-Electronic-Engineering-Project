#ifndef MUSICRESOURCE_H
#define MUSICRESOURCE_H

#include "Base.h"

namespace de
{
    namespace resources
    {
        class MusicResource
        {
            public:
                //! Basic Constructor.
                MusicResource() : Music(0) {}
                //! Constructor with FileName passed in.
                MusicResource( const std::string &_name, const boost::filesystem::path &_path );
                ~MusicResource();

                const int& getMusic();
                void load();
                void unload();
            private:
                int Music;
                std::string name;
                boost::filesystem::path path;
        };
	}
}
#endif //MUSICRESOURCE_H

