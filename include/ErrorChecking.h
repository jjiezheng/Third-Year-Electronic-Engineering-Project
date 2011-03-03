#ifndef ERRORCHECKING_H
#define ERRORCHECKING_H

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

#endif // ERRORCHECKING_H
