#include "HapticsOpject.h"
#include "Haptics.h"
#include "GameServices.h"

#include "openGL.h"/*
#include <aiTypes.h>
#include <aiScene.h> 
#include <aiPostProcess.h>
#include <assimp.hpp>*/

hapticsOpject::hapticsOpject(void)
{
	hapticShape = hlGenShapes(1);
}


hapticsOpject::~hapticsOpject(void)
{
}



hapticsOpject& hapticsOpject::writeToDepth( bool _writeToDepth )
{
	mesh.writeToDepth( _writeToDepth );
	return *this;
}

hapticsOpject& hapticsOpject::depth( bool _depth )
{
	mesh.depth( _depth );
	return *this;
}

hapticsOpject& hapticsOpject::alpha( bool _alpha )
{
	mesh.alpha( _alpha );
	return *this;
}

hapticsOpject& hapticsOpject::blend( bool _blend )
{
	mesh.blend( _blend );
	return *this;
}

hapticsOpject& hapticsOpject::setType( const std::string &_type )
{
	mesh.setType( _type );
	return *this;
}

hapticsOpject& hapticsOpject::load( const std::string &_mesh, const std::string &_shader )
{
	mesh.load( _mesh, _shader );
	/*
	Assimp::Importer importer;

	std::string path( de::filesystem::Roots->get( de::filesystem::root::MESHES )+_mesh );
	de::io::tests << "-------------------------------------------------------\n";
	de::io::tests << "Loading medel from:" << path << "\n";
	const aiScene* scene = importer.ReadFile( path.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals );

	if( !scene )
	{
		de::io::error << importer.GetErrorString() << "\n";
		de::io::tests << "-------------------------------------------------------\n";
	}

	if( scene->HasMeshes() )
	{
		de::io::tests << "It has " << scene->mNumMeshes << " meshes!" << "\n";

		aiMesh* _mesh = scene->mMeshes[0];
		de::io::tests << "Mesh has " <<  _mesh->mNumVertices << " Verts and contains ";
		/*
		vbo.Attribs["Position"] = de::graphics::AttInfo( 4, 
			VertexBuffer.size()*sizeof(de::graphics::vertex), 
			sizeof(de::graphics::vertex),
			0 );
			
		int i, num = _mesh->mNumVertices;
		aiVector3D* verts = _mesh->mVertices;
		for( i = 0; i!=num; ++i)
		{
			VertexBuffer.push_back( de::graphics::vertex( verts[i].x, verts[i].y, verts[i].z, 1.0f ) );
		}
		de::io::tests << "VertexBuffer.size() = " << VertexBuffer.size() << "\n";

		num = _mesh->mNumFaces;
		aiFace* faces = _mesh->mFaces;
		for( i = 0; i!=num; ++i)
		{
			ElementBuffer.push_back( faces[i].mIndices[0] );
			ElementBuffer.push_back( faces[i].mIndices[1] );
			ElementBuffer.push_back( faces[i].mIndices[2] );
		}
		de::io::tests << "ElementBuffer.size() = " << ElementBuffer.size() << "\n";

	}
	*/
	return *this;
}

hapticsOpject& hapticsOpject::setTexture( const std::string &_texture, const std::string &_value )
{
	mesh.setTexture( _texture, _value );
	return *this;
}


void hapticsOpject::reload()
{
	mesh.reload();
}

void hapticsOpject::render()
{
	de::Engine::Graphics().add( &mesh );
	de::Engine::Graphics().add( this, 1 );
}

void hapticsOpject::actualRender()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// combined graphic and haptic rendering pass

	hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, hapticShape);
	//mesh.actualRender();
	hlEndShape();
}

