#ifndef SHADERMODULE_H
#define SHADERMODULE_H

#include "Base.h"
#include "Material.h"

namespace de
{
    namespace graphics
    {
        class ShaderModule
        {
            public:
                /** Default constructor */
                ShaderModule();
                /** Default destructor */
                virtual ~ShaderModule();

                bool load( lua_State* L, Shader &_shader, const std::string &_name, glslVersion::glslVersions _version );
                bool unload( Shader &_shader );
            protected:
            private:
                std::string getShaderName( const std::string &_name, const std::string &_shaderType, const std::string &_glsl );

                bool compileShader( const std::string &_name, const char *_shaderText, unsigned int &_shader, const int &_type );

                bool linkProgram( const std::string &_name, unsigned int &_shaderHandle, unsigned int &_vertexShader, unsigned int &_fragShader );

                void parseShader( const char* _shaderText,
                                  const std::string &_type,
                                  std::map<std::string, AttributeTypes > &_Attributes );

                void parseShader( const char* _shaderText,
                                  const std::string &_type,
                                  std::map<std::string, ShaderVariant > &_Uniforms );

                const char* GetShaderSource(lua_State* L, const char* effectKey);
        };
    }
}

#endif // SHADERMODULE_H
