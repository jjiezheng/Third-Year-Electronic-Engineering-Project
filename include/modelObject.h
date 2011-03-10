#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "Base.h"
#include "renderObject.h"

#include "VBO.h"
#include "Material.h"



class modelObject : public de::graphics::renderObject
{
    public:
        /** Default constructor */
        modelObject();
        /** Default destructor */
        virtual ~modelObject();

        template<typename T>
        modelObject& setUniform( const std::string &_name, const T &_value )
        {
            shader.setUniform( _name, _value );
            return *this;
        }
        modelObject& writeToDepth( bool _writeToDepth );
		modelObject& depth( bool _depth );
		modelObject& alpha( bool _alpha );
		modelObject& blend( bool _blend );

        modelObject& setType( const std::string &_type );
        modelObject& load( const std::string &_mesh, const std::string &_shader );
		modelObject& setTexture( const std::string &_texture, const std::string &_value );

        void reload();
        void render();
		void actualRender();
    private:
		

        bool usingTexture, writingToDepth, depthTest, alphaTest, blending, active;
        int type, texture;
		std::string shaderName, textureName, meshName;

		de::graphics::VBO vbo;
        de::graphics::Shader shader;
		std::vector<std::string> AttributeNames;
		std::vector<std::string>::iterator iterNames;
        
};


#endif //MODELOBJECT_H