#ifndef COREGRAPHICS_H
#define COREGRAPHICS_H


#include "Vector.h"
#include "Base.h"


namespace de
{
    namespace graphics
    {
        enum AttributeTypes
        {
            INT,
            FLOAT = 1,
            VEC2 = 2,
            VEC3 = 3,
            VEC4 = 4,
            MAT3,
            MAT4,
        };

        namespace glVersion
        {
            enum glVersions
            {
                none = 0,
                GL_1_1 = 1,
                GL_1_2 = 1 << 2,
                GL_1_3 = 1 << 3,
                GL_1_4 = 1 << 4,
                GL_1_5 = 1 << 5,
                GL_2_0 = 1 << 6,
                GL_2_1 = 1 << 7,
                GL_3_0 = 1 << 8,
                GL_3_1 = 1 << 9,
                GL_3_2 = 1 << 10,
                GL_3_3 = 1 << 11,
                GL_4_0 = 1 << 12,
                GL_4_1 = 1 << 13,
                GL_NEWER = 1 << 14
            };
        }
        namespace glslVersion
        {
            enum glslVersions
            {
                none = 0,
                GLSL_110 = glVersion::GL_2_0,
                GLSL_120 = glVersion::GL_2_1,
                GLSL_130 = glVersion::GL_3_0,
                GLSL_140 = glVersion::GL_3_1,
                GLSL_150 = glVersion::GL_3_2,
                GLSL_330 = glVersion::GL_3_3,
                GLSL_400 = glVersion::GL_4_0,
                GLSL_410 = glVersion::GL_4_1,
                GLSL_NEWER = glVersion::GL_NEWER
            };
        }

        std::string glToString( glVersion::glVersions _glversion );
        std::string glToString( glslVersion::glslVersions _glslversion );
        std::string glToToken( glslVersion::glslVersions _glslversion );

        glVersion::glVersions getOpenGLVersion( const std::string &_version );
        glslVersion::glslVersions getGLSLVersion( const std::string &_version );

        typedef boost::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4> ShaderVariant;
    }
}


#endif // COREGRAPHICS_H
