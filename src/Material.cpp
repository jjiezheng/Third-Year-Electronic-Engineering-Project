#include "Material.h"

#include "openGL.h"
#include "BindShader.h"

namespace de
{
    namespace graphics
    {
        UniformValue::UniformValue( const Uint32 &_shader, const std::string &_name, const ShaderVariant &_value )
        {
            name = _name;
            value = _value;
            reload( _shader );
        }

		UniformValue::~UniformValue()
		{
			name.clear();
		}
        void UniformValue::bind()
        {
            boost::apply_visitor( BindShader(), uniformAddress, value );
        }

        void UniformValue::reload( const Uint32 &_shader )
        {
            uniformAddress = glGetUniformLocation( _shader, name.c_str() );
            CHECKGL_MSG( "glGetUniformLocation" );
        }



        AttributeValue::AttributeValue( const Uint32 &_shader, const std::string &_name, const AttributeTypes &_type )
        {
            name = _name;
            type = _type;
            reload( _shader );
        }

		AttributeValue::~AttributeValue()
		{
			name.clear();
		}
        int AttributeValue::get( AttributeTypes &_type )
        {
            _type = type;
            return attributeAddress;
        }

        void AttributeValue::reload( const Uint32 &_shader )
        {
            attributeAddress = glGetAttribLocation( _shader, name.c_str() );
            CHECKGL_MSG( "glGetUniformLocation" );
        }



        Shader::Shader():
            active(false), name("No name"), shaderHandle(0), vertexShader(0), fragShader(0)
        {
            //ctor
        }

        Shader::Shader( const std::string &_shaderName,
                        const std::map<std::string, ShaderVariant > &_Uniforms,
                        const std::map<std::string, AttributeTypes > &_Attributes,
                        const Uint32 &_shaderHandle,
                        const Uint32 &_vertexShader,
                        const Uint32 &_fragShader ):
            active(false), name("No name"), shaderHandle(0), vertexShader(0), fragShader(0)
        {
            setup( _shaderName, _Uniforms, _Attributes, _shaderHandle, _vertexShader, _fragShader );
        }

        Shader::~Shader()
        {
            shaderHandle = vertexShader = fragShader = 0;
            Uniforms.clear();
            Attributes.clear();
        }

        void Shader::setup( const std::string &_shaderName,
                            const std::map<std::string, ShaderVariant > &_Uniforms,
                            const std::map<std::string, AttributeTypes > &_Attributes,
                            const Uint32 &_shaderHandle,
                            const Uint32 &_vertexShader,
                            const Uint32 &_fragShader )
        {
            active = true;
            name = _shaderName;

            Uniforms.clear();
            Attributes.clear();
            shaderHandle = _shaderHandle;
            vertexShader = _vertexShader;
            fragShader = _fragShader;

            {
                std::map<std::string, AttributeTypes >::const_iterator iter, end = _Attributes.end();
                for( iter = _Attributes.begin(); iter != end; ++iter )
                    Attributes[iter->first] =  AttributeValue( shaderHandle, iter->first, iter->second );
            }

            {
                std::map<std::string, ShaderVariant >::const_iterator iter, end = _Uniforms.end();
                for( iter = _Uniforms.begin(); iter != end; ++iter )
                    Uniforms[iter->first] =  UniformValue( shaderHandle, iter->first, iter->second );
            }

			AttributesIter = Attributes.begin();
			UniformsIter = Uniforms.begin();
        }
        bool Shader::isOK()
        {
            return active;
        }

        void Shader::bindShader()
        {
            if( !active )
            {
                de::io::error << "Shader is NOT set up!\n";
                return;
            }

            CHECKGL( glUseProgram( shaderHandle ) );

            if( Uniforms.empty() )
                return;

            for( UniformsIter = Uniforms.begin(); UniformsIter != Uniforms.end(); ++UniformsIter )
                CHECKGL( UniformsIter->second.bind() );
        }

        void Shader::bindUniforms()
        {
            for( UniformsIter = Uniforms.begin(); UniformsIter != Uniforms.end(); ++UniformsIter )
                CHECKGL( UniformsIter->second.bind() );
        }

        void Shader::logic( const Uint32 &_deltaTicks )
        {

        }

        const Uint32& Shader::get( shaderType _type ) const
        {
            if(  _type == PROGRAM )
            {
                return shaderHandle;
            }
            else if( _type == FRAG )
            {
                return fragShader;
            }
            else if( _type == VERTEX )
            {
                return vertexShader;
            }

            static GLuint nullAndVoid(0);
            return nullAndVoid;
        }

        int Shader::getAttribute( const std::string &_name, AttributeTypes &_type )
        {
            if( Attributes.empty() )
            {
                de::io::error << "Error: Request for Shader Attribute - " << _name << " when no Attributes exist.\n";
                return 0;
            }


            AttributesIter = Attributes.find( _name );
            if( AttributesIter != Attributes.end() )
            {
                return AttributesIter->second.get( _type );
            }

            de::io::error << "Error: Request for Shader Attribute - " << _name << " Which doesn't exist.\n";
            return 0;
        }

        int Shader::getAttribute( const std::string &_name )
        {
            if( Attributes.empty() )
            {
                de::io::error << "Error: Request for Shader Attribute - " << _name << " when no Attributes exist.\n";
                return 0;
            }


            AttributesIter = Attributes.find( _name );
            if( AttributesIter != Attributes.end() )
            {
                AttributeTypes type;
                return AttributesIter->second.get( type );
            }

            de::io::error << "Error: Request for Shader Attribute - " << _name << " Which doesn't exist.\n";
            return 0;
        }

        std::vector<std::string> Shader::getAttributeNames()
        {
            std::vector<std::string> temp;

            for( AttributesIter = Attributes.begin(); AttributesIter != Attributes.end(); ++AttributesIter )
                temp.push_back( AttributesIter->first );

            return temp;
        }

        void Shader::clear()
        {
            if( get( Shader::PROGRAM ) )
            {
                CHECKGL( glDetachShader( get( Shader::PROGRAM ), get( Shader::VERTEX ) ) );
                CHECKGL( glDetachShader( get( Shader::PROGRAM ), get( Shader::FRAG ) ) );

                CHECKGL( glDeleteShader( get( Shader::VERTEX ) ) );
                CHECKGL( glDeleteShader( get( Shader::FRAG ) ) );

                CHECKGL( glDeleteProgram( get( Shader::PROGRAM ) ) );
            }
        }
    }
}
