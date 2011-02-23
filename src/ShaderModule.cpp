#include "ShaderModule.h"
#include "CoreEnumsAndClasses.h"

#include "openGL.h"

namespace de
{
    namespace graphics
    {
        ShaderModule::ShaderModule()
        {
            //ctor
        }

        ShaderModule::~ShaderModule()
        {
            //dtor
        }



        std::string ShaderModule::getShaderName( const std::string &_name, const std::string &_shaderType, const std::string &_glsl )
        {
            std::vector<std::string> strs;
            std::vector<std::string>::iterator iter, end;

            boost::split(strs, _name, boost::is_any_of("."));
            iter = strs.begin();
            end = strs.end();

            std::string subname, start = (*iter);
            for( iter = strs.begin()+1; iter < end; ++iter )
            {
                subname = "." + (*iter);
            }

            return ( start + _shaderType + _glsl + subname );
        }

        bool ShaderModule::compileShader( const std::string &_name, const char *_shaderText, unsigned int &_shader, const int &_type )
        {
            _shader = glCreateShader(_type);
            CHECKGL_MSG( "glCreateShader(_type)" );

            CHECKGL( glShaderSource( _shader, 1, &_shaderText, NULL ) );
            CHECKGL( glCompileShader( _shader ) );

            GLint shaderOk(0);
            glGetShaderiv(_shader, GL_COMPILE_STATUS, &shaderOk);
            if(!shaderOk)
            {
                GLsizei size(0);
                char log[300];
                de::io::error << "Failed to compile " << _name << "\n";
                glGetShaderInfoLog( _shader, 300, &size, log );
                de::io::error << log << "\n";
                de::io::error << "Dumping shader...\n " << _shaderText << "\n";
                return false;
            }
            return true;
        }

        bool ShaderModule::linkProgram( const std::string &_name, unsigned int &_shaderHandle, unsigned int &_vertexShader, unsigned int &_fragShader )
        {
            _shaderHandle  = glCreateProgram();
            CHECKGL_MSG( "glCreateProgram");

            CHECKGL( glAttachShader( _shaderHandle, _vertexShader ) );
            CHECKGL( glAttachShader( _shaderHandle, _fragShader ) );
            CHECKGL( glLinkProgram( _shaderHandle ) );

            GLint programOk(0);
            glGetProgramiv(_shaderHandle, GL_LINK_STATUS, &programOk);
            if(!programOk)
            {
                GLsizei size(0);
                char log[300];

                de::io::error << "Failed to Link " << _name << "\n" << "Printing error message\n";

                glGetProgramInfoLog( _shaderHandle, 300, &size, log );
                de::io::error << log << "\n";
                return false;
            }
            return true;
        }



        void ShaderModule::parseShader( const char* _shaderText,
                                    const std::string &_type,
                                    std::map<std::string, AttributeTypes > &_Attributes )
        {
            std::map<std::string, AttributeTypes >::iterator iter, end;

            std::stringstream s(_shaderText);
            std::string tmp, varname, type;
            while( s.good() )
            {
                s >> tmp;
                if(tmp == _type)
                {
                    s >> type >> varname; //Get rid of the ";"
                    unsigned int index = varname.find(";");
                    if(index != std::string::npos)
                    {
                        varname.resize(index);
                    }
                    if( type == "float" )
                    {
                        _Attributes[varname] = FLOAT;
                    }
                    else if( type == "sampler2D" ||  type == "int" )
                    {
                        _Attributes[varname] = INT;
                    }
                    else if( type == "vec2" )
                    {
                        _Attributes[varname] = VEC2;
                    }
                    else if( type == "vec3" )
                    {
                        _Attributes[varname] = VEC3;
                    }
                    else if( type == "vec4" )
                    {
                        _Attributes[varname] = VEC4;
                    }
                    else if( type == "mat3" )
                    {
                        _Attributes[varname] = MAT3;
                    }
                    else if( type == "mat4" )
                    {
                        _Attributes[varname] = MAT4;
                    }
                }
            }
        }

        void ShaderModule::parseShader( const char* _shaderText,
                                    const std::string &_type,
                                    std::map<std::string, ShaderVariant > &_Uniforms )
        {
            std::map<std::string, ShaderVariant >::iterator iter, end;

            std::stringstream s(_shaderText);
            std::string tmp, varname, type;
            while( s.good() )
            {
                s >> tmp;
                if(tmp == _type)
                {
                    s >> type >> varname; //Get rid of the ";"
                    unsigned int index = varname.find(";");
                    if(index != std::string::npos)
                    {
                        varname.resize(index);
                    }
                    if( type == "float" )
                    {
                        _Uniforms[varname] = static_cast<float>(1.0f);
                    }
                    else if( type == "sampler2D" ||  type == "int" )
                    {
                        _Uniforms[varname] = static_cast<int>(0);
                    }
                    else if( type == "vec2" )
                    {
                        _Uniforms[varname] = glm::vec2( 1.0f );
                    }
                    else if( type == "vec3" )
                    {
                        _Uniforms[varname] = glm::vec3( 1.0f );
                    }
                    else if( type == "vec4" )
                    {
                        _Uniforms[varname] = glm::vec4( 1.0f );
                    }
                    else if( type == "mat3" )
                    {
                        _Uniforms[varname] = glm::mat3( 1.0f );
                    }
                    else if( type == "mat4" )
                    {
                        _Uniforms[varname] = glm::mat4( 1.0f );
                    }
                }
            }
        }

        const char* ShaderModule::GetShaderSource(lua_State* L, const char* effectKey)
        {
            // Extract the effect name:
            const char* targetKey = strchr(effectKey, '.');
            if (!targetKey++)
                return 0;

            char effectName[32] = {0};
            strncpy(effectName, effectKey, targetKey - effectKey - 1);

            // Fetch the table from the Lua context and make sure it exists:
            lua_getglobal(L, effectName);
            if (!lua_istable(L, -1))
            {
                lua_pop(L, 1);

                // Delay-load the Lua file:
                char effectPath[64];
                std::string path( de::filesystem::Roots->get( de::filesystem::root::SHADERS ) +  effectName + ".lua" );
                sprintf(effectPath, path.c_str() );
                if(luaL_dofile(L, effectPath))
                {
                    de::io::log << "The shader file named " << effectPath << " doesn't exist\n";
                    return 0;
                }

                // If it's still not there, give up!
                lua_getglobal(L, effectName);
                if (!lua_istable(L, -1))
                {
                    de::io::log << "The shader named " << effectName << " doesn't exist in " << effectPath <<"\n";
                    exit(1);
                }
            }

            const char* closestMatch = 0;
            int closestMatchLength = 0;

            int i = lua_gettop(L);
            lua_pushnil(L);
            while (lua_next(L, i) != 0)
            {
                const char* shaderKey = lua_tostring(L, -2);
                int shaderKeyLength = strlen(shaderKey);

                // Find the longest key that matches the beginning of the target key:
                if (strstr(targetKey, shaderKey) != 0 && shaderKeyLength > closestMatchLength)
                {
                    closestMatchLength = shaderKeyLength;
                    closestMatch = lua_tostring(L, -1);
                }
                lua_pop(L, 1);
            }

            lua_pop(L, 1);
            return closestMatch;
        }

        bool ShaderModule::load( lua_State* L, Shader &_shader, const std::string &_name, glslVersion::glslVersions _version )
        {
            std::string glsl = glToToken( _version );
            std::string name(  _name +  "." + glsl  );

            std::string fragmentShaderName(  getShaderName( _name, ".Fragment.", glsl )  );
            std::string vertexShaderName(  getShaderName( _name, ".Vertex.", glsl )  );

            const char* fragmentShaderText = GetShaderSource( L, fragmentShaderName.c_str() );
            const char* vertexShaderText = GetShaderSource( L, vertexShaderName.c_str() );


            GLuint vertexShader, fragShader, ShaderHandle;
            if( !compileShader( vertexShaderName, vertexShaderText, vertexShader, GL_VERTEX_SHADER ) ||
                !compileShader( vertexShaderName, fragmentShaderText, fragShader, GL_FRAGMENT_SHADER ) )
            {
                return false;
            }

            if( !linkProgram( name, ShaderHandle, vertexShader, fragShader ) )
            {
                return false;
            }


            std::map<std::string, ShaderVariant > Uniforms;
            std::map<std::string, AttributeTypes > Attributes;

            parseShader( fragmentShaderText, "uniform", Uniforms );
            parseShader( vertexShaderText, "uniform", Uniforms );

            if( _version >= glslVersion::GLSL_130 )
                parseShader( vertexShaderText, "in", Attributes );
            else
                parseShader( vertexShaderText, "attribute", Attributes );

            _shader.setup( name, Uniforms, Attributes,  ShaderHandle, vertexShader, fragShader );

            return true;
        }

        bool ShaderModule::unload( Shader &_shader )
        {
            _shader.clear();
            return true;
        }

    }
}
