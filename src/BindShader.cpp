#include "BindShader.h"

#include "openGL.h"
#include "ErrorChecking.h"

void BindShader::operator()( int &_uniformAddress, float &_value ) const
{
    CHECKGL( glUniform1f( _uniformAddress, _value ) );
}

void BindShader::operator()( int &_uniformAddress, int &_value ) const
{
    CHECKGL( glUniform1i( _uniformAddress, _value ) );
}

void BindShader::operator()( int &_uniformAddress, glm::vec2 &_value ) const
{
    CHECKGL( glUniform2fv( _uniformAddress, 1, glm::value_ptr(_value) ) );
}

void BindShader::operator()( int &_uniformAddress, glm::vec3 &_value ) const
{
    CHECKGL( glUniform3fv( _uniformAddress, 1, glm::value_ptr(_value) ) );
}

void BindShader::operator()( int &_uniformAddress, glm::vec4 &_value ) const
{
    CHECKGL( glUniform4fv( _uniformAddress, 1, glm::value_ptr(_value) ) );
}

void BindShader::operator()( int &_uniformAddress, glm::mat3 &_value ) const
{
    CHECKGL( glUniformMatrix3fv( _uniformAddress, 1, GL_FALSE, glm::value_ptr(_value) ) );
}

void BindShader::operator()( int &_uniformAddress, glm::mat4 &_value ) const
{
    CHECKGL( glUniformMatrix4fv( _uniformAddress, 1, GL_FALSE, glm::value_ptr(_value) ) );
}



