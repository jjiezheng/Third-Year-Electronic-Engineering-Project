#ifndef MESHRESOURCE_H
#define MESHRESOURCE_H

#include "Base.h"
#include <assimp.hpp>

#include "VBO.h"
#include "renderObject.h"

namespace de
{
    namespace resources
    {
        class MeshResource
        {
            public:
                MeshResource() {}
                MeshResource( const std::string &_path, const std::string &_name );
                ~MeshResource();

				bool load( const std::string &_path, const std::string &_name );
				graphics::VBO& get();
			private:
				bool loadWithAssimp( const std::string &_path, const std::string &_name );
				void parseWithAssimp( const aiScene* _scene );

				std::vector<de::graphics::vertex> VertexBuffer;
				std::vector< std::vector<de::graphics::vertex> >::iterator iter;

				std::vector<int> ElementBuffer;
				std::vector< std::vector<int> >::iterator iterElement;

				Assimp::Importer importer;
				graphics::VBO vbo;
				std::string name;
        };
	}
}
#endif //MESHRESOURCE_H

