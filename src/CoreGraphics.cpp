#include "CoreGraphics.h"

namespace de
{
    namespace graphics
    {
        std::string glToString( graphics::glVersion::glVersions _glversion )
        {
            using namespace glVersion;

            switch( _glversion )
            {
                case GL_1_1:
                    return "1.1";
                case GL_1_2:
                    return "1.2";
                case GL_1_3:
                    return "1.3";
                case GL_1_4:
                    return "1.4";
                case GL_1_5:
                    return "1.5";
                case GL_2_0:
                    return "2.0";
                case GL_2_1:
                    return "2.1";
                case GL_3_0:
                    return "3.0";
                case GL_3_1:
                    return "3.1";
                case GL_3_2:
                    return "3.1";
                case GL_3_3:
                    return "3.3";
                case GL_4_0:
                    return "4.0";
                case GL_4_1:
                    return "4.1";
                case GL_NEWER:
                    return "newer than 4.1";
                default:
                    return "Somthing has gone wrong";
            }
            return "BLUP";
        }

        std::string glToString( graphics::glslVersion::glslVersions _glslversion )
        {
            using namespace glslVersion;

            switch( _glslversion )
            {
                case GLSL_110:
                    return "1.10";
                case GLSL_120:
                    return "1.20";
                case GLSL_130:
                    return "1.30";
                case GLSL_140:
                    return "1.40";
                case GLSL_150:
                    return "1.50";
                case GLSL_330:
                    return "3.30";
                case GLSL_400:
                    return "4.00";
                case GLSL_410:
                    return "4.10";
                case GLSL_NEWER:
                    return "newer than 4.10";
                default:
                    return "NONE";
            }
            return "BLUP";
        }

        std::string glToToken( graphics::glslVersion::glslVersions _glslversion )
        {
            using namespace glslVersion;

            switch( _glslversion )
            {
                case GLSL_110:
                    return "GL20";
                case GLSL_120:
                    return "GL21";
                case GLSL_130:
                    return "GL30";
                case GLSL_140:
                    return "GL31";
                case GLSL_150:
                    return "GL32";
                case GLSL_330:
                    return "GL33";
                case GLSL_400:
                    return "GL40";
                case GLSL_410:
                    return "GL41";
                case GLSL_NEWER:
                    return "GL41";
                default:
                    return "NONE";
            }
            return "BLUP";
        }

        glVersion::glVersions getOpenGLVersion( const std::string &_version )
        {
            using namespace glVersion;


            int majorVersion(0), minorVersion(0);
            std::vector<std::string> strs;

            boost::split( strs, _version, boost::is_any_of(".") );

            if( strs.size() == 1 )
            {
                majorVersion = boost::lexical_cast<int>(strs[0]);
            }
            else if( strs.size() >= 2 )
            {
                majorVersion = boost::lexical_cast<int>(strs[0]);
                minorVersion = boost::lexical_cast<int>(strs[1]);
            }

            switch( majorVersion )
            {
                case 1:
                    switch( minorVersion )
                    {
                        case 1: return GL_1_1;
                        case 2: return GL_1_2;
                        case 3: return GL_1_3;
                        case 5: return GL_1_4;
                        case 4: return GL_1_5;
                        default : return GL_1_5;
                    }
                case 2:
                    switch( minorVersion )
                    {
                        case 0: return GL_2_0;
                        case 1: return GL_2_1;
                        default : return GL_2_0;
                    }
                case 3:
                    switch( minorVersion )
                    {
                        case 0: return GL_3_0;
                        case 1: return GL_3_1;
                        case 2: return GL_3_2;
                        case 3: return GL_3_3;
                        default : return GL_3_0;
                    }
                case 4:
                    switch( minorVersion )
                    {
                        case 0: return GL_4_0;
                        case 1: return GL_4_1;
                        default : return GL_4_0;
                    }
                default: if( majorVersion >= 4 ) return GL_NEWER;
            }

            return none;
        }


        glslVersion::glslVersions getGLSLVersion( const std::string &_version )
        {
            using namespace glslVersion;


            int majorVersion(0), minorVersion(0);
            std::vector<std::string> strs;

            boost::split( strs, _version, boost::is_any_of(".") );

            if( strs.size() == 1 )
            {
                majorVersion = boost::lexical_cast<int>(strs[0]);
            }
            else if( strs.size() >= 2 )
            {
                majorVersion = boost::lexical_cast<int>(strs[0]);
                minorVersion = boost::lexical_cast<int>(strs[1]);
            }

            switch( majorVersion )
            {
                case 2:
                    switch( minorVersion )
                    {
                        case 0: return GLSL_110;
                        case 1: return GLSL_120;
                        default : return GLSL_110;
                    }
                case 3:
                    switch( minorVersion )
                    {
                        case 0: return GLSL_130;
                        case 1: return GLSL_140;
                        case 2: return GLSL_150;
                        case 3: return GLSL_330;
                        default : return GLSL_130;
                    }
                case 4:
                    switch( minorVersion )
                    {
                        case 0: return GLSL_400;
                        case 1: return GLSL_410;
                        default : return GLSL_400;
                    }
                default: if( majorVersion >= 4 ) return GLSL_NEWER;
            }

            return none;
        }

    }
}
