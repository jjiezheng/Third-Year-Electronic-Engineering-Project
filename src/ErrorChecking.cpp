#include "ErrorChecking.h"

#include "Base.h"
#include "Log.h"
#include "openGL.h"


std::string de::luaError::error = "no error yet";
std::string de::luaError::lineNumber = "no error yet";
std::string de::luaError::file = "no error yet";
std::string de::luaError::message = "no error yet";

int CheckGLError(const char *GLcall, const char *file, int line)
{
    boost::filesystem::path filePath( file );
    GLenum glErr;
    int    retCode = 0;
    while ( (glErr=glGetError()) != GL_NO_ERROR)
    {
        switch(glErr)
        {
            case GL_INVALID_ENUM:
                de::io::error << "GL_INVALID_ENUM error in File " << filePath.filename() << " at line: " << line << "\n";
                break;
            case GL_INVALID_VALUE:
                de::io::error << "GL_INVALID_VALUE error in File " << filePath.filename() << " at line: " << line << "\n";
                break;
            case GL_INVALID_OPERATION:
                de::io::error << "GL_INVALID_OPERATION error in File " << filePath.filename() << " at line: " << line << "\n";
                break;
            case GL_STACK_OVERFLOW:
                de::io::error << "GL_STACK_OVERFLOW error in File " << filePath.filename() << " at line: " << line << "\n";
                break;
            case GL_STACK_UNDERFLOW:
                de::io::error << "GL_STACK_UNDERFLOW error in File " << filePath.filename() << " at line: " << line << "\n";
                break;
            case GL_OUT_OF_MEMORY:
                de::io::error << "GL_OUT_OF_MEMORY error in File " << filePath.filename() << " at line: " << line << "\n";
                break;
            default:
                de::io::error << "UNKNOWN error in File " << filePath.filename() << " at line: " << line << "\n";
        }
    }
    return retCode;
};

int add_file_and_line(lua_State* L)
{
   std::string err = lua_tostring(L, -1);
   lua_pop(L, 1);


	std::vector<std::string> strs;
	std::vector<std::string>::iterator iter, end;
	boost::split( strs, err, boost::is_any_of(":") );

	if( strs.size() >= 3 )
    {
		de::luaError::file = strs[0];
		de::luaError::lineNumber = strs[1];
		de::luaError::error = strs[2];
		de::luaError::message = de::luaError::error+":"+de::luaError::lineNumber;
	}
	
	lua_pushstring(L, de::luaError::message.c_str() );
	return 1;
}

int stackDump(lua_State *L) 
{
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) 
	{ /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) 
		{
			case LUA_TSTRING: 
			{ /* strings */
				printf("'%s'", lua_tostring(L, i));
				de::io::error <<  lua_tostring(L, i);
				break;
			}
			case LUA_TBOOLEAN: 
			{ /* booleans */
				printf(lua_toboolean(L, i) ? "true" : "false");
				de::io::error << (lua_toboolean(L, i) ? "true" : "false");
				break;
			}
			case LUA_TNUMBER: 
			{ /* numbers */
				printf("%g", lua_tonumber(L, i));
				de::io::error <<  lua_tonumber(L, i);
				break;
			}
			default: 
			{ /* other values */
				printf("%s", lua_typename(L, t));
				de::io::error <<  lua_typename(L, t);
				break;
			}
		}
		printf(" "); /* put a separator */
		de::io::error << " ";
	}
	printf("\n"); /* end the listing */
	de::io::error << "\n";
	return 1;
}