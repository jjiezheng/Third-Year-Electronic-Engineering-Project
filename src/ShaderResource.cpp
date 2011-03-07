#include "ShaderResource.h"
#include "GameServices.h"


namespace de
{
    namespace resources
    {

        ShaderResource::ShaderResource() {}

        ShaderResource::ShaderResource( const std::string &_name )
        {
            name = _name;
        }

        ShaderResource::~ShaderResource()
        {
            unload();
        }

        const de::graphics::Shader& ShaderResource::getShaderObject( lua_State* L )
        {
            if( shader.get( graphics::Shader::PROGRAM ) == 0 )
            {
                loadUsing( L );
            }
            return shader;
        }

        bool ShaderResource::loadUsing( lua_State* L )
        {
            Engine::Graphics().loadShader( L, shader, name );
            return true;
        };

        void ShaderResource::unload()
        {
            Engine::Graphics().unloadShader( shader );
        }
	}
}