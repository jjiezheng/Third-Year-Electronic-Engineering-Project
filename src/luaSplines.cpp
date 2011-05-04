#include "luaSplines.h"
#include <glm/gtx/spline.hpp>


namespace de
{
    namespace luaInterface
    {
        void exportSplines( lua_State *_luaState )
        {
            luabind::module(_luaState )
            [
				luabind::def( "catmullRom", (glm::vec2(*)( const glm::vec2&,const glm::vec2&,const glm::vec2&,const glm::vec2&, const float&)) &::glm::catmullRom ),
				luabind::def( "cubic",		(glm::vec2(*)( const glm::vec2&,const glm::vec2&,const glm::vec2&,const glm::vec2&, const float&)) &::glm::cubic ),
				luabind::def( "hermite",	(glm::vec2(*)( const glm::vec2&,const glm::vec2&,const glm::vec2&,const glm::vec2&, const float&)) &::glm::hermite ),

				luabind::def( "catmullRom", (glm::vec3(*)( const glm::vec3&,const glm::vec3&,const glm::vec3&,const glm::vec3&, const float&)) &::glm::catmullRom ),
				luabind::def( "cubic",		(glm::vec3(*)( const glm::vec3&,const glm::vec3&,const glm::vec3&,const glm::vec3&, const float&)) &::glm::cubic ),
				luabind::def( "hermite",	(glm::vec3(*)( const glm::vec3&,const glm::vec3&,const glm::vec3&,const glm::vec3&, const float&)) &::glm::hermite ),

				luabind::def( "catmullRom", (glm::vec4(*)( const glm::vec4&,const glm::vec4&,const glm::vec4&,const glm::vec4&, const float&)) &::glm::catmullRom ),
				luabind::def( "cubic",		(glm::vec4(*)( const glm::vec4&,const glm::vec4&,const glm::vec4&,const glm::vec4&, const float&)) &::glm::cubic ),
				luabind::def( "hermite",	(glm::vec4(*)( const glm::vec4&,const glm::vec4&,const glm::vec4&,const glm::vec4&, const float&)) &::glm::hermite )
            ];
        }
	}
}