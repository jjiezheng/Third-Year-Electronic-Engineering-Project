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
			de::io::log << "-------------------------------------------------------\n";
			de::io::log << "Loading medel from:" << path << "\n";
			const aiScene* scene = importer.ReadFile( path.c_str(),
				aiProcess_CalcTangentSpace |
				aiProcess_Triangulate |
				aiProcess_SortByPType |
				aiProcess_GenSmoothNormals );

			if( !scene )
			{
				de::io::error << importer.GetErrorString() << "\n";
				de::io::log << "-------------------------------------------------------\n";
				return false;
			}

			if( scene->HasMeshes() )
			{
				parseVertices( scene );
				parseAnimations( scene );
				makeVBO();
			}

			de::io::log << "-------------------------------------------------------\n";
			return true;
		}


		void MeshResource::parseVertices( const aiScene* _scene )
		{
			de::io::log << "It has " << _scene->mNumMeshes << " meshes!" << "\n";
			aiMesh* _mesh = _scene->mMeshes[0];

			if( _mesh->HasPositions() )
			{
				parsePositions( _scene );
			}

			if( _mesh->HasNormals() )
			{
				parseNormals( _scene );
			}

			if( _mesh->GetNumUVChannels() && _scene->HasMaterials() )
			{
				parseUVs( _scene );
			}

			if( _mesh->HasBones() )
			{
				parseBones( _scene );				
			}
			else
			{
				de::io::log << "No bones!\n";
			}

		}

		void MeshResource::parsePositions( const aiScene* _scene )
		{
			aiMesh* _mesh = _scene->mMeshes[0];
			de::io::log << "Mesh has " <<  _mesh->mNumVertices << " Verts and contains ";
			if( _mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE )
			{
				de::io::log << "Triangles\n";
			}

			vbo.Attribs["Position"] = graphics::AttInfo( 4, 
														 VertexBuffer.size()*sizeof(de::graphics::vertex), 
														 sizeof(de::graphics::vertex),
														 0 );

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

		void MeshResource::parseNormals( const aiScene* _scene )
		{
			aiMesh* _mesh = _scene->mMeshes[0];
			de::io::log << "Mesh has normals\n";
			vbo.Attribs["Normal"] = graphics::AttInfo(  4, 
														VertexBuffer.size()*sizeof(de::graphics::vertex), 
														sizeof(de::graphics::vertex),
														0 );

			int i, num = _mesh->mNumVertices;
			aiVector3D* verts = _mesh->mNormals;
			for( i = 0; i!=num; ++i)
			{
				VertexBuffer.push_back( graphics::vertex( verts[i].x, verts[i].y, verts[i].z, 1.0f ) );
			}
		}

		void MeshResource::parseUVs( const aiScene* _scene )
		{
			aiMesh* _mesh = _scene->mMeshes[0];
			aiMaterial* _material = _scene->mMaterials[_mesh->mMaterialIndex];


			aiString nameOfMaterial;
			aiTextureMapping textureMappingType;
			unsigned int uvIndex;
			float blend;
			aiTextureOp texOp;
			aiTextureMapMode mapMode;

			_material->GetTexture( aiTextureType_DIFFUSE,
								  0,
								  &nameOfMaterial,
								  &textureMappingType,
								  &uvIndex,
								  &blend,
								  &texOp,
								  &mapMode );


			int numberOfUVs =  _mesh->GetNumUVChannels();
			std::string texture( nameOfMaterial.data );
			de::io::log << "Mesh has " << numberOfUVs << " UVs.\n";
			de::io::log << "diffuse texture is " << de::filesystem::stripFileEnding( texture ) << "\n";

			vbo.textures.push_back( de::filesystem::stripFileEnding( texture ) );
			for( int i = 0; i != numberOfUVs; i++ )
			{
				std::string uvNumber(de::misc::toString(i));
				vbo.Attribs["UV_"+uvNumber] = graphics::AttInfo( 2, 
																 VertexBuffer.size()*sizeof(de::graphics::vertex),
																 sizeof(de::graphics::vertex),
																 1 );

				int j, num = _mesh->mNumVertices;
				aiVector3D* verts = _mesh->mNormals;
				for( j = 0; j!=num; ++j)
				{
					VertexBuffer.push_back( graphics::vertex( verts[j].x, verts[j].y, 0.0f, 0.0f ) );
				}
			}
		}

		void MeshResource::parseBones( const aiScene* _scene )
		{/*
			aiMesh* _mesh = _scene->mMeshes[0];
			de::io::log << "Mesh has bones\n";
			aiBone * _bone = _mesh->mBones[0];

				
			std::string name = _bone->mName.data;
			de::io::log << "Animation name:" << name << "\n";
			de::io::log << "mNumWeights:" << _bone->mNumWeights << "\n\n";

			for(int i = 0; i != _bone->mNumWeights; ++i )
			{
				de::io::log << "mVertexId:" << _bone->mWeights[i].mVertexId << "\n";
				de::io::log << "mWeight:" << _bone->mWeights[i].mWeight << "\n";
			}
			de::io::log << "\n";*/
		}

		void MeshResource::parseAnimations( const aiScene* _scene )
		{/*
			if(  !_scene->HasAnimations() )
			{
				return;
			}
			de::io::log << "\nIt has animation\n";

			aiAnimation *_animations =  _scene->mAnimations[0];

			std::string name = _animations->mName.data;
			de::io::log << "Animation name:" << name << "\n";
			de::io::log << "Duration:" << _animations->mDuration << "\n";
			de::io::log << "Ticks per second:" << _animations->mTicksPerSecond << "\n";
			de::io::log << "Number of Channels:" << _animations->mNumChannels << "\n\n";

			aiNodeAnim * _nodeAnimation = _animations->mChannels[0];
			*/
		}
		void MeshResource::makeVBO()
		{
			vbo.meshName = name;
			vbo.triangles = ElementBuffer.size();
			vbo.vertexBuffer = make_buffer( GL_ARRAY_BUFFER, &(VertexBuffer[0]), VertexBuffer.size()*sizeof(de::graphics::vertex) );
			vbo.elementBuffer = make_buffer( GL_ELEMENT_ARRAY_BUFFER, &(ElementBuffer[0]), ElementBuffer.size()*sizeof(int) );

			if( name == "world.3ds" )
			{
				de::io::tests << "vbo.vertexBuffer: " << vbo.vertexBuffer << " vbo.elementBuffer: " << vbo.elementBuffer << "\n";
			}
		}
	}
}