#ifndef ERRORCHECKING_H
#define ERRORCHECKING_H

#include <string>

int CheckGLError(const char *GLcall, const char *file, int line);
/*
#define CHECKGL( GLcall )                        \
{                                                \
    GLcall;                                      \
    if(1)                                        \
    CheckGLError( #GLcall, __FILE__, __LINE__ ); \
}

#define CHECKGL_MSG( msg )                       \
{                                                \
    if(1)                                        \
    CheckGLError( #msg, __FILE__, __LINE__ );    \
}*/

#define CHECKGL( GLcall )                        \
{                                                \
    GLcall;                                      \
}

#define CHECKGL_MSG( msg )                       \
{                                                \
}


struct lua_State;
int add_file_and_line(lua_State* L);
int stackDump(lua_State *L);

namespace de
{
	class luaError
	{
	public:
		static std::string error;
		static std::string lineNumber;
		static std::string file;
		static std::string message;
	};
}



#endif // ERRORCHECKING_H
