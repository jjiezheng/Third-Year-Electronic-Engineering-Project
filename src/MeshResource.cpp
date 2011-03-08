#include "MeshResource.h"
#include "GameServices.h"
#include "CoreEnumsAndClasses.h"

#include "openGL.h"
#include <aiTypes.h>
#include <aiScene.h> 
#include <aiPostProcess.h>

namespace
{
    static Uint32 make_buffer( GLenum target, const void *buffer_data, GLsizei buffer_size )
    {
        Uint32 buffer;
        CHECKGL( glGenBuffers( 1, &buffer ) );
        CHECKGL( glBindBuffer( target, buffer ) );
        CHECKGL( glBufferData( target, buffer_size, buffer_data, GL_STATIC_DRAW ) );
        return buffer;
    }
}


namespace de
{
    namespace resources
    {

        MeshResource::MeshResource( const std::string &_path, const std::string &_name )
		{
			name = _name;
			path = _path;
			//load();
		}

		MeshResource::~MeshResource()
		{
			unload();
		}

		graphics::VBO& MeshResource::get()
		{
			return vbo;
		}

		bool MeshResource::load()
		{
			unload();
			return loadWithAssimp( path, name );
		}

		bool MeshResource ::unload()
		{
			VertexBuffer.clear();
			iter = VertexBuffer.begin();

			ElementBuffer.clear();
			iterElement = ElementBuffer.begin();

			return true;
		}

		bool MeshResource::loadWithAssimp( const std::string &_path, const std::string &_name )
		{
			using namespace de::filesystem;
			using namespace de::graphics;

			Assimp::Importer importer;

			std::string path( _path+_name );
			de::io::log << "path:" << path << "\n";
			const aiScene* scene = importer.ReadFile( path.c_str(),
				aiProcess_CalcTangentSpace |
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_GenSmoothNormals |
				aiProcess_LimitBoneWeights |
				aiProcess_RemoveRedundantMaterials |
				aiProcess_ImproveCacheLocality |
				aiProcess_OptimizeMeshes );

			if( !scene )
			{
				de::io::error << importer.GetErrorString() << "\n";
				return false;
			}

			if( scene->HasMeshes() )
			{
				parseWithAssimp( scene );
				makeVBO();
			}

			if(  scene->HasAnimations() )
			{
				de::io::log << "It has animation\n";
			}

			return true;
		}


		void MeshResource::parseWithAssimp( const aiScene* _scene )
		{
			de::io::log << "It has " << _scene->mNumMeshes << " meshes!" << "\n";
			aiMesh* _mesh = _scene->mMeshes[0];

			if( _mesh->HasPositions() )
			{
				de::io::log << "Mesh has " <<  _mesh->mNumVertices << " Verts and contains ";
				if( _mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE )
				{
					de::io::log << "Triangles\n";
				}

				vbo.Attribs["Position"] = graphics::AttInfo( _mesh->mNumVertices, 
															 VertexBuffer.size()*sizeof(de::graphics::vertex), 
															 sizeof(de::graphics::vertex) );

				int i, num = _mesh->mNumVertices;
				aiVector3D* verts = _mesh->mVertices;
				for( i = 0; i!=num; ++i)
				{
					VertexBuffer.push_back( graphics::vertex( verts[i].x, verts[i].y, verts[i].z, 1.0f ) );
				}
				de::io::log << "VertexBuffer.size() = " << VertexBuffer.size() << "\n";

				num = _mesh->mNumFaces;
				aiFace* faces = _mesh->mFaces;
				for( i = 0; i!=num; ++i)
				{
					ElementBuffer.push_back( faces[i].mIndices[0] );
					ElementBuffer.push_back( faces[i].mIndices[1] );
					ElementBuffer.push_back( faces[i].mIndices[2] );
				}
				de::io::log << "ElementBuffer.size() = " << ElementBuffer.size() << "\n";
			}

			if( _mesh->HasNormals() )
			{
				de::io::log << "Mesh has normals\n";
				vbo.Attribs["Normal"] = graphics::AttInfo( _mesh->mNumVertices, 
														   VertexBuffer.size()*sizeof(de::graphics::vertex), 
														   sizeof(de::graphics::vertex) );

				int i, num = _mesh->mNumVertices;
				aiVector3D* verts = _mesh->mNormals;
				for( i = 0; i!=num; ++i)
				{
					VertexBuffer.push_back( graphics::vertex( verts[i].x, verts[i].y, verts[i].z, 1.0f ) );
				}
			}


			de::io::log << "Mesh has " << _mesh->GetNumUVChannels() << " UVs.\n";

			if( _mesh->HasBones() )
			{
				de::io::log << "Mesh has bones\n";
			}

		}

		void MeshResource::makeVBO()
		{
			vbo.meshName = name;
			vbo.triangles = ElementBuffer.size();
			vbo.vertexBuffer = make_buffer( GL_ARRAY_BUFFER, &(VertexBuffer[0]), VertexBuffer.size()*sizeof(de::graphics::vertex) );
			vbo.elementBuffer = make_buffer( GL_ELEMENT_ARRAY_BUFFER, &(ElementBuffer[0]), ElementBuffer.size()*sizeof(int) );
		}
	}
}