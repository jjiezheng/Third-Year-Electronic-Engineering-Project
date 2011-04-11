#include "Object3D.h"
#include "GameServices.h"
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
        CHECKGL( glBufferData( target, buffer_size, buffer_data, GL_DYNAMIC_DRAW ) );
        return buffer;
    }
}



Object3D::Object3D()
    : refreshVBO(false), usingTexture(false), writingToDepth(true), alphaTest(true), blending(true), created(false), type( GL_TRIANGLES )
{
}

Object3D::~Object3D()
{
	clear();
}

void Object3D::clear()
{
	CHECKGL( glDeleteBuffers( 1, &vertexBuffer ) );
	CHECKGL( glDeleteBuffers( 1, &elementBuffer ) );

	VertexBuffer.clear();
	ElementBuffer.clear();
	AttributeInfo.clear();
	AttributeElemenRange.clear();
	AttributeNames.clear();

	refreshVBO = true;
}

Object3D& Object3D::set( const std::string &_shaderName )
{
	AttributeNames.clear();
	AttributeInfo.clear();
	AttributeElemenRange.clear();

	AttIter = AttributeInfo.begin();

    shaderName = _shaderName;
    shader = de::Engine::Resources().getShader( shaderName );
    AttributeNames = shader.getAttributeNames();

    std::vector<std::string>::iterator start, end = AttributeNames.end();
    for( start = AttributeNames.begin(); start != end; ++start )
    {
        AttributeInfo[(*start)] = bufferInfo();
        AttributeElemenRange[(*start)] = bufferInfo();
    }

    return *this;
}

Object3D& Object3D::setTexture( const std::string &_name, const std::string &_value )
{
    textureName = _value;
    //shader.setUniform( _name, Engine::Resources().getTexture(_value) );
	shader.setUniform( _name, 0 );
    usingTexture = true;
    texture = de::Engine::Resources().getTexture(_value);

    return *this;
}

Object3D& Object3D::writeToDepth( bool _writeToDepth )
{
    writingToDepth = _writeToDepth;
    return *this;
}
Object3D& Object3D::depth( bool _depth )
{
	depthTest = _depth;
	return *this;
}
Object3D& Object3D::alpha( bool _alpha )
{
	alphaTest = _alpha;
	return *this;
}
Object3D& Object3D::blend( bool _blend )
{
	blending = _blend;
	return *this;
}




Object3D& Object3D::add( const std::string &_name, const std::vector<de::graphics::vertex> &_vertexBuffer, const std::vector<int> &_elementBuffer )
{
	ElementBuffer.clear();
    ElementBuffer = _elementBuffer;
    add( _name, _vertexBuffer );

    return *this;
}

Object3D& Object3D::add( const std::string &_name, const std::vector<de::graphics::vertex> &_vertexBuffer )
{
    std::map<std::string, bufferInfo>::iterator find = AttributeInfo.find( _name );
    if( find == AttributeInfo.end() || !find->second.isSet )
    {
        de::graphics::AttributeTypes type;
        shader.getAttribute( _name, type );
        AttributeInfo[_name] = bufferInfo( VertexBuffer.size()*sizeof(de::graphics::vertex), _vertexBuffer.size(), type );

        for( std::vector<de::graphics::vertex>::const_iterator vec4Iter = _vertexBuffer.begin(); vec4Iter != _vertexBuffer.end(); ++vec4Iter )
        {
            VertexBuffer.push_back( (*vec4Iter) );
        }
        refreshVBO = true;
    }
    else
    {
        if( find->second.size == _vertexBuffer.size() )
        {
            int i(0);
            for( std::vector<de::graphics::vertex>::const_iterator vec4Iter = _vertexBuffer.begin(); vec4Iter != _vertexBuffer.end(); ++vec4Iter, ++i )
            {
                VertexBuffer[find->second.size + i] = (*vec4Iter);
            }
            refreshVBO = true;
        }
    }
    return *this;
}

Object3D& Object3D::setType( const std::string &_type )
{
    if( _type == "TriangleStrip" )
        type = GL_TRIANGLE_STRIP;
    else if( _type == "Triangles" )
        type = GL_TRIANGLES;
    else if( _type == "Point" )
        type = GL_POINT;
    else if( _type == "Points" )
        type = GL_POINTS;

    return *this;
}


Object3D& Object3D::load( const std::string &_name, const std::string &_shader )
{
	return loadAssimp( _name, _shader );
}

Object3D& Object3D::loadAssimp( const std::string &_name, const std::string &_shader )
{
    using namespace de::filesystem;
    using namespace de::graphics;

	set( _shader );
    type = GL_TRIANGLES;
    refreshVBO = true;
    meshName = _name;

    std::string path( Roots->get( root::MESHES )+meshName );
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
		return *this;
    }

	if( scene->HasMeshes() )
	{
		de::io::log << "It has " << scene->mNumMeshes << " meshes!" << "\n";


		aiMesh* _mesh = scene->mMeshes[0];

		
		if( _mesh->HasPositions() )
		{
			de::io::log << "Mesh has " <<  _mesh->mNumVertices << " Verts and contains ";
			if( _mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE )
			{
				de::io::log << "Triangles\n";
			}
			
			de::graphics::AttributeTypes attType;
			shader.getAttribute( "Position", attType );
			AttributeInfo["Position"] = bufferInfo( VertexBuffer.size()*sizeof(de::graphics::vertex), _mesh->mNumVertices, attType );

			int i, num = _mesh->mNumVertices;
			aiVector3D* verts = _mesh->mVertices;
			for( i = 0; i!=num; ++i)
			{
				VertexBuffer.push_back( vertex( verts[i].x, verts[i].y, verts[i].z, 1.0f ) );
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
			
			
			de::graphics::AttributeTypes attTypeNormal;
			shader.getAttribute( "Normal", attTypeNormal );
			AttributeInfo["Normal"] = bufferInfo( VertexBuffer.size()*sizeof(de::graphics::vertex), _mesh->mNumVertices, attTypeNormal );

			int i, num = _mesh->mNumVertices;
			aiVector3D* verts = _mesh->mNormals;
			for( i = 0; i!=num; ++i)
			{
				VertexBuffer.push_back( vertex( verts[i].x, verts[i].y, verts[i].z, 1.0f ) );
			}
		}


		de::io::log << "Mesh has " << _mesh->GetNumUVChannels() << " UVs.\n";

		if( _mesh->HasBones() )
		{
			de::io::log << "Mesh has bones\n";
		}

	}

	if(  scene->HasAnimations() )
	{
		de::io::log << "It has animation\n";
	}
    return *this;
}


void Object3D::makeBuffers()
{
	if( created )
	{
		CHECKGL( glDeleteBuffers( 1, &vertexBuffer ) );
		CHECKGL( glDeleteBuffers( 1, &elementBuffer ) );
	}
    vertexBuffer = make_buffer( GL_ARRAY_BUFFER, &(VertexBuffer[0]), VertexBuffer.size()*sizeof(de::graphics::vertex) );
    elementBuffer = make_buffer( GL_ELEMENT_ARRAY_BUFFER, &(ElementBuffer[0]), ElementBuffer.size()*sizeof(int) );
    refreshVBO = false;
	created = true;
}

void Object3D::reload()
{
    refreshVBO = true;
    shader.setUniform( "Texture0", de::Engine::Resources().getTexture(textureName) );

    texture = de::Engine::Resources().getTexture(textureName);
    shader = de::Engine::Resources().getShader( shaderName );
}

void Object3D::render()
{
    de::Engine::Graphics().add( this );
}

void Object3D::actualRender()
{
    if( refreshVBO )
    {
        makeBuffers();
    }

    if( usingTexture )
    {
        CHECKGL( glEnable( GL_TEXTURE_2D ) );
        CHECKGL( glActiveTexture( GL_TEXTURE0 ) );
        CHECKGL( glBindTexture( GL_TEXTURE_2D, texture ) );
    }
    else
        CHECKGL( glDisable( GL_TEXTURE_2D ) );

    if( writingToDepth )
	{
		CHECKGL( glDepthMask( GL_TRUE ) );
	}
    else glDepthMask( GL_FALSE );

	if( depthTest ) 
	{
		CHECKGL( glEnable( GL_DEPTH_TEST ) );
	}
    else CHECKGL( glDisable( GL_DEPTH_TEST ) );

	if( alphaTest ) 
	{
		CHECKGL( glEnable( GL_ALPHA_TEST ) );
	}
    else CHECKGL( glDisable( GL_ALPHA_TEST ) );

	if( blending ) 
	{
		CHECKGL( glEnable( GL_BLEND ) );
	}
    else CHECKGL( glDisable( GL_BLEND ) );


    shader.bindShader();
    CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer ) );
    for( AttIter = AttributeInfo.begin(); AttIter != AttributeInfo.end(); ++AttIter )
    {
        if( AttIter->second.isSet )
        {
            int size(AttIter->second.type);

            CHECKGL( glVertexAttribPointer(
                shader.getAttribute( AttIter->first ),        // attribute
                size,                                         // size
                GL_FLOAT,                                     // type
                GL_FALSE,                                     // normalized?
                sizeof(de::graphics::vertex),                 // stride
                (void*)(AttributeInfo[AttIter->first].offset) // array buffer offset
            ) );
            CHECKGL( glEnableVertexAttribArray( shader.getAttribute( AttIter->first) ) );
        }
        else if( AttIter->second.warning )
        {
            AttIter->second.warning = false;
            de::io::error << "Attribute missing:\"" << AttIter->first << "\". Using Shader:\"" << shaderName << "\" Mesh:\"" << meshName << "\"\n";
        }
    }

    CHECKGL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementBuffer ) );
        CHECKGL( glDrawElements(
            type,  // mode
            ElementBuffer.size(),                  // count
            GL_UNSIGNED_INT,    // type
            (void*)0            // element array buffer offset
        ) );

    for( AttIter = AttributeInfo.begin(); AttIter != AttributeInfo.end(); ++AttIter )
    {
        CHECKGL( glDisableVertexAttribArray( shader.getAttribute( AttIter->first ) ) );
    }
    //CHECKGL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
    //CHECKGL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    CHECKGL( glUseProgram( 0 ) );
}
