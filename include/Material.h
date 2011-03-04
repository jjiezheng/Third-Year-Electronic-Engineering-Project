#ifndef MATERIAL_H
#define MATERIAL_H

#include "CoreGraphics.h"
#include "BindShader.h"


namespace de
{
    namespace graphics
    {
        class UniformValue
        {
            public:
                UniformValue() {};
                UniformValue( const Uint32 &_shader, const std::string &_name, const ShaderVariant &_value );
                ~UniformValue();

                template <typename T>
                void changeValue( const T &_value )
                {
                    boost::variant<T> local( _value );
                    boost::apply_visitor( ChangeButKeepType(), value, local );
                }
                void changeValue( const ShaderVariant &_value )
                {
                    value = _value;
                    //boost::apply_visitor( ChangeButKeepType(), value, _value );
                }

                void bind();
                void reload( const Uint32 &_shader );

            private:
                boost::variant<int> uniformAddress;
                ShaderVariant value;
                std::string name;
        };

        class AttributeValue
        {
            public:
                AttributeValue() {};
                AttributeValue( const Uint32 &_shader, const std::string &_name, const AttributeTypes &_type );
                ~AttributeValue();

                int get( AttributeTypes &_type );
                void reload( const Uint32 &_shader );

            private:
                int attributeAddress;
                AttributeTypes type;
                std::string name;
        };

        class Shader
        {
            public:
                enum shaderType
                {
                    FRAG,
                    VERTEX,
                    PROGRAM
                };

                Shader();
                Shader( const std::string &_shaderName,
                        const std::map<std::string, ShaderVariant > &_Uniforms,
                        const std::map<std::string, AttributeTypes > &_Attributes,
                        const Uint32 &_shaderHandle,
                        const Uint32 &_vertexShader,
                        const Uint32 &_fragShader );
                virtual ~Shader();

                void setup( const std::string &_shaderName,
                            const std::map<std::string, ShaderVariant > &_Uniforms,
                            const std::map<std::string, AttributeTypes > &_Attributes,
                            const Uint32 &_shaderHandle,
                            const Uint32 &_vertexShader,
                            const Uint32 &_fragShader );

                template<typename T>
                bool setUniform( const std::string &_name, const T &_value )
                {
                    if( Uniforms.empty() )
                        return false;

                    UniformsIter = Uniforms.find( _name );
                    if( UniformsIter != Uniforms.end() )
                    {
                        UniformsIter->second.changeValue( _value );
                        return true;
                    }
                    return false;
                }

                void logic( const Uint32 &_deltaTicks );
                void bindShader();
                void bindUniforms();
                bool isOK();
                const Uint32& get( shaderType _type ) const;
                int getAttribute( const std::string &_name, AttributeTypes &_type );
                int getAttribute( const std::string &_name );
                std::vector<std::string> getAttributeNames();
                void clear();


            protected:
            private:
                bool active;
                std::string name;
                Uint32 shaderHandle, vertexShader, fragShader;


                std::map<std::string, AttributeValue> Attributes;
                std::map<std::string, AttributeValue>::iterator AttributesIter;

                std::map<std::string, UniformValue> Uniforms;
                std::map<std::string, UniformValue>::iterator UniformsIter;
        };


    }
}


#endif // MATERIAL_H
