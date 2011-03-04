#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Material.h"
#include "renderObject.h"
#include "CoreEnumsAndClasses.h"
#include <assimp.hpp>


struct bufferInfo
{
    unsigned int offset;
    unsigned int size;
    de::graphics::AttributeTypes type;
    bool isSet, warning;

    bufferInfo() : offset(0), size(0), isSet(false), warning(true) {}
    bufferInfo( int _offset, int _size, de::graphics::AttributeTypes _type ) : offset(_offset), size(_size), type(_type), isSet(true), warning(false) {}
};

class Object3D : public de::graphics::renderObject
{
    public:
        /** Default constructor */
        Object3D();
        /** Default destructor */
        virtual ~Object3D();
        Object3D& set( const std::string &_shaderName );

        template<typename T>
        Object3D& setUniform( const std::string &_name, const T &_value )
        {
            shader.setUniform( _name, _value );
            return *this;
        }
        Object3D& setTexture( const std::string &_name, const std::string &_value );
        Object3D& writeToDepth( bool _writeToDepth );
		Object3D& depth( bool _depth );
		Object3D& alpha( bool _alpha );
		Object3D& blend( bool _blend );

        Object3D& add( const std::string &_name, const std::vector<de::graphics::vertex> &_vertexBuffer, const std::vector<int> &_elementBuffer );
        Object3D& add( const std::string &_name, const std::vector<de::graphics::vertex> &_vertexBuffer );
        Object3D& setType( const std::string &_type );
		Object3D& mode( int _modes );
        Object3D& load( const std::string &_name, const std::string &_shader );


        void reload();
        void render();
        void localRender();
		void clear();

    private:
        void makeBuffers();
        Object3D& loadAssimp( const std::string &_name, const std::string &_shader );


        bool refreshVBO, usingTexture, writingToDepth, depthTest, alphaTest, blending;
        int type, texture;
        de::graphics::Shader shader;
        Uint32 vertexBuffer, elementBuffer;
        std::string shaderName, textureName, meshName;

		Assimp::Importer importer;

        std::vector<std::string> AttributeNames;
        std::vector<de::graphics::vertex> VertexBuffer;
        std::vector< std::vector<de::graphics::vertex> >::iterator iter;

        std::map<std::string, bufferInfo> AttributeInfo;
        std::map<std::string, bufferInfo>::iterator AttIter;

        std::vector<int> ElementBuffer;
        std::vector< std::vector<int> >::iterator iterElement;
        std::map<std::string, bufferInfo> AttributeElemenRange;
};

#endif // OBJECT3D_H
