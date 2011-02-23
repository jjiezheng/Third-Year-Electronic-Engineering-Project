#include "ErrorChecking.h"


#include "Log.h"

#include "openGL.h"
#include <string>
#include <boost/filesystem.hpp>


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
