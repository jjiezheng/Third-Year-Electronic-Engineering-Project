#include "HapticsOpject.h"
#include "Haptics.h"
#include "GameServices.h"

#include "openGL.h"
#include <aiTypes.h>
#include <aiScene.h> 
#include <aiPostProcess.h>
#include <assimp.hpp>
#include "HLU/hlu.h"
#include "HDU/hdu.h"

GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
                      1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
                      1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
                      -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                      -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
                      1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

GLfloat vertices_new[21600];

GLubyte indices[] = {0,1,2,3,
                     4,5,6,7,
                     8,9,10,11,
                     12,13,14,15,
                     16,17,18,19,
                     20,21,22,23};



hapticsOpject::hapticsOpject(void) : loaded(false)
{
	hapticShape = hlGenShapes(1);
}


hapticsOpject::~hapticsOpject(void)
{
}


hapticsOpject& hapticsOpject::setUniform( const std::string &_name, const glm::mat4 &_value )
{
	if( _name == "Projection" )
	{
		projection = _value;
	}
	else if( _name == "View" )
	{
		view = _value;
	}
	else if( _name == "Model" )
	{
		model = _value;
	}

	mesh.setUniform( _name, _value );
	return *this;
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
	
	Assimp::Importer importer;
	loaded = true;

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
			*/
		int i, num = _mesh->mNumVertices;
		aiVector3D* verts = _mesh->mVertices;
		for( i = 0; i!=num; ++i)
		{
			VertexBuffer.push_back( verts[i].x );			
			VertexBuffer.push_back( verts[i].y );			
			VertexBuffer.push_back( verts[i].z );
		}
		for( i = 0; i!=21600; ++i)
		{
			vertices_new[i] = VertexBuffer[i];			
		}


		de::io::tests << "VertexBuffer.size() = " << VertexBuffer.size()/3 << "\n";

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
	if( loaded )
	{

		glClear(GL_DEPTH_BUFFER_BIT);
		// Cull back faces.
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf( glm::value_ptr(projection) );
		glMatrixMode(GL_MODELVIEW_MATRIX);
		glLoadMatrixf( glm::value_ptr(model*view) );

		//hluFitWorkspace(glm::value_ptr(glm::dmat4(projection)));
		hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, 0.7f);
		hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, 0.1f);
		hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, 0.2f);
		hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, 0.3f);
		hlHinti(HL_SHAPE_FEEDBACK_BUFFER_VERTICES, 7200);

		//hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, hapticShape);
		hlBeginShape(HL_SHAPE_DEPTH_BUFFER, hapticShape);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer( 3, GL_FLOAT, 0, vertices_new );
			glDrawArrays(GL_TRIANGLES, 0, 7200);
			glDisableClientState(GL_VERTEX_ARRAY);  
		hlEndShape();
	}
}

