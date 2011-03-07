#include "MeshResource.h"
#include "GameServices.h"
#include "CoreEnumsAndClasses.h"

#include <aiTypes.h>
#include <aiScene.h> 
#include <aiPostProcess.h>

namespace de
{
    namespace resources
    {

        MeshResource::MeshResource( const std::string &_path, const std::string &_name )
		{
			name = _name;
			load( _path, _name );
		}

		MeshResource::~MeshResource()
		{

		}

		graphics::VBO& MeshResource::get()
		{
			return vbo;
		}

		bool MeshResource::load( const std::string &_path, const std::string &_name )
		{
			return loadWithAssimp( _path, _name );
		}

		bool MeshResource::loadWithAssimp( const std::string &_path, const std::string &_name )
		{
			using namespace de::filesystem;
			using namespace de::graphics;

			Assimp::Importer importer;

			//std::string path( Roots->get( root::MESHES )+_name );
			std::string path( _path+_name );
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
		/*
				de::graphics::AttributeTypes attType;
				shader.getAttribute( "Position", attType );
				AttributeInfo["Position"] = bufferInfo( VertexBuffer.size()*sizeof(de::graphics::vertex), _mesh->mNumVertices, attType );
				*/
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
			
		/*
				de::graphics::AttributeTypes attTypeNormal;
				AttributeInfo["Normal"] = bufferInfo( VertexBuffer.size()*sizeof(de::graphics::vertex), _mesh->mNumVertices, attTypeNormal );
				*/
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
	}
}