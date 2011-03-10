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

				bool load();
				bool unload();
				graphics::VBO& get();
			private:
				bool loadWithAssimp( const std::string &_path, const std::string &_name );
				void parseVertices( const aiScene* _scene );
				void parsePositions( const aiScene* _scene );
				void parseNormals( const aiScene* _scene );
				void parseUVs( const aiScene* _scene );
				void parseBones( const aiScene* _scene );

				void parseAnimations( const aiScene* _scene );
				void makeVBO();

				std::vector<de::graphics::vertex> VertexBuffer;
				std::vector<de::graphics::vertex>::iterator iter;

				std::vector<int> ElementBuffer;
				std::vector<int>::iterator iterElement;

				
				graphics::VBO vbo;
				std::string name, path;
        };
	}
}
#endif //MESHRESOURCE_H

