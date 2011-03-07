#ifndef SHADERRESOURCE_H
#define SHADERRESOURCE_H

#include "Material.h"
#include "Base.h"

namespace de
{
    namespace resources
    {
        class ShaderResource
        {
            public:
                //! Constructor with FileName passed in.
                ShaderResource();
                ShaderResource( const std::string &_name );
                ~ShaderResource();

                //! Loads shader using the Lua Shader Factory
                bool loadUsing( lua_State* L );
                //! Unloads the Shader Program
                void unload();

                //! Get shader Object
                const graphics::Shader& getShaderObject( lua_State* L );


            private:
                graphics::Shader shader;
                std::string name;
        };
	}
}
#endif //SHADERRESOURCE_H

