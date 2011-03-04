#ifndef SKYBOX_H
#define SKYBOX_H

#include "Material.h"
#include <renderObject.h>
#include <base.h>


class SkyBox : public de::graphics::renderObject
{
    public:
        /** Default constructor */
        SkyBox();
        /** Default destructor */
        virtual ~SkyBox();

        enum
        {
            FRONT = 0,
            BACK = 1,
            LEFT = 2,
            RIGHT = 3,
            TOP = 4,
            BOTTOM = 5
        } Face;

        template<typename T>
        SkyBox& setUniform( const std::string &_name, const T &_value )
        {
            shader.setUniform( _name, _value );
            return *this;
        }

        SkyBox& add( const std::string &_name, int _face );
        SkyBox& set( const std::string &_shaderName );
        void reload();
        void render();
        void localRender();
    protected:
    private:
        de::graphics::Shader shader;
        std::string shaderName, textureName[6];
        int texture[6];
        bool textureSet[6], refreshVBO;


        Uint32 vertexBuffer[6], elementBuffer;


        std::vector<std::string> AttributeNames;
        std::vector<de::graphics::vertex> VertexBuffer;
        std::vector< std::vector<de::graphics::vertex> >::iterator iter;
/*
        std::map<std::string, bufferInfo> AttributeInfo;
        std::map<std::string, bufferInfo>::iterator AttIter;

        std::vector<int> ElementBuffer;
        std::vector< std::vector<int> >::iterator iterElement;
        std::map<std::string, bufferInfo> AttributeElemenRange;*/
};

#endif // SKYBOX_H
