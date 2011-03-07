#ifndef MESHRESOURCE_H
#define MESHRESOURCE_H

#include "Base.h"
#include "VBO.h"

namespace de
{
    namespace resources
    {
        class MeshResource
        {
            public:
                MeshResource() {}
                MeshResource( const std::string &_name, const boost::filesystem::path &_path );
                ~MeshResource();

        };
	}
}
#endif //MESHRESOURCE_H

