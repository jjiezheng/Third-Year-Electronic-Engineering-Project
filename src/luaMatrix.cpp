#include "luaMatrix.h"

#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <glm/gtx/spline.hpp>


namespace
{
    float vec2X( glm::vec2* _vec ) { return _vec->x; }
    float vec2Y( glm::vec2* _vec ) { return _vec->y; }

    float change_vec2X( glm::vec2* _vec, const float &_new ) { _vec->x = _new; return _vec->x; }
    float change_vec2Y( glm::vec2* _vec, const float &_new ) { _vec->y = _new; return _vec->y; }

    float vec3X( glm::vec3* _vec ) { return _vec->x; }
    float vec3Y( glm::vec3* _vec ) { return _vec->y; }
    float vec3Z( glm::vec3* _vec ) { return _vec->z; }

    float change_vec3X( glm::vec3* _vec, const float &_new ) { _vec->x = _new; return _vec->x; }
    float change_vec3Y( glm::vec3* _vec, const float &_new ) { _vec->y = _new; return _vec->y; }
    float change_vec3Z( glm::vec3* _vec, const float &_new ) { _vec->z = _new; return _vec->z; }

}
namespace de
{
    namespace luaInterface
    {
        void exportMatrix( lua_State *_luaState )
        {
            luabind::module(_luaState )
            [
                luabind::class_<glm::vec2>( "vec2" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor<const float& >() )
                    .def( luabind::constructor<const float& ,const float& >() )
					.def( "x", &vec2X )
                    .def( "y", &vec2Y )
                    .def( "set_x", &change_vec2X )
                    .def( "set_y", &change_vec2Y )
                    .def( luabind::const_self * glm::vec2() )
					.def( luabind::const_self / glm::vec2() )
					.def( luabind::const_self + glm::vec2() )
					.def( luabind::const_self - glm::vec2() )
					.def( luabind::self/float() )
                    .def( luabind::self*float() ),

                luabind::class_<glm::vec3>( "vec3" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float& >() )
                    .def( luabind::constructor< const float&, const float&, const float& >() )
                    .def( "x", &vec3X )
                    .def( "y", &vec3Y )
                    .def( "z", &vec3Z )
                    .def( "set_x", &change_vec3X )
                    .def( "set_y", &change_vec3Y )
                    .def( "set_z", &change_vec3Z )
                    .def( luabind::self*glm::vec3() )
                    .def( luabind::self/float() )
                    .def( luabind::self*float() )
                    .def( luabind::self*glm::mat3() )
					.def( glm::mat3()*luabind::self )
                    .def( luabind::self-glm::vec3() )
                    .def( luabind::self+glm::vec3() ),

                luabind::class_<glm::vec4>( "vec4" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float& >() )
                    .def( luabind::constructor< const float&, const float&, const float&, const float& >() )
                    .def( luabind::constructor< const glm::vec3&, const float& >() )
					.def( luabind::self/float() )
                    .def( luabind::self*float() )
                    .def( luabind::self*glm::mat4() )
                    .def( luabind::self+glm::vec4() )
                    .def( luabind::self-glm::vec4() )
                    .def( luabind::self*glm::vec4() ),

                luabind::class_<de::graphics::vertex>( "vertex" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float&, const float&, const float&, const float& >() ),

                luabind::class_<glm::mat3>( "mat3" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float& >() )
                    .def( luabind::constructor< glm::mat4 >() )
                    .def( luabind::self*glm::mat3() ),

                luabind::class_<glm::mat4>( "mat4" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float& >() )
                    .def( luabind::constructor< glm::mat3 >() )
                    .def( luabind::self*glm::mat4() ),

                luabind::class_<glm::quat>( "quat" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float&, const glm::vec3&>() ),


                luabind::def( "normalise", (de::graphics::vertex(*)   ( const de::graphics::vertex&)) &de::graphics::normalise ),
                luabind::def( "normalise", (glm::vec2(*)( const glm::vec2&)) &::glm::normalize ),
                luabind::def( "normalise", (glm::vec3(*)( const glm::vec3&)) &::glm::normalize ),
                luabind::def( "normalise", (glm::vec4(*)( const glm::vec4&)) &::glm::normalize ),

				luabind::def( "length", (float(*)( const glm::vec2&)) &::glm::length ),
                luabind::def( "length", (float(*)( const glm::vec3&)) &::glm::length ),
                luabind::def( "length", (float(*)( const glm::vec4&)) &::glm::length ),

                luabind::def( "cross", (glm::vec3(*)( const glm::vec3&,const glm::vec3&)) &::glm::cross ),

				luabind::def( "dot", (float(*)( const glm::vec2&,const glm::vec2&)) &::glm::dot ),
                luabind::def( "dot", (float(*)( const glm::vec3&,const glm::vec3&)) &::glm::dot ),
                luabind::def( "dot", (float(*)( const glm::vec4&,const glm::vec4&)) &::glm::dot ),

                luabind::def( "mix", (float(*)( const float&,const float&,const float&)) &::glm::mix ),
                luabind::def( "mix", (glm::vec3(*)( const glm::vec3&,const glm::vec3&,const float&)) &::glm::mix ),
                luabind::def( "mix", (glm::vec4(*)( const glm::vec4&,const glm::vec4&,const float&)) &::glm::mix ),
                luabind::def( "mix", (glm::quat(*)( const glm::quat&,const glm::quat&,const float&)) &::glm::mix ),
                luabind::def( "smoothstep", (glm::vec3(*)( const glm::vec3&,const glm::vec3&,const glm::vec3&)) &::glm::smoothstep ),
                luabind::def( "smoothstep", (float(*)( const float&,const float&,const float&)) &::glm::smoothstep ),

                luabind::def( "ortho", (glm::mat4(*)( float const&, float const&, float const&, float const& )) &::glm::ortho ),
                luabind::def( "perspective", (glm::mat4(*)( float const&, float const&, float const&, float const&)) &::glm::perspective ),
                luabind::def( "lookAt",      (glm::mat4(*)( glm::vec3 const&, glm::vec3 const&, glm::vec3 const& )) &::glm::lookAt ),
                luabind::def( "translate",   (glm::mat4(*)( glm::vec3 const& )) &::glm::translate ),
                luabind::def( "translate",   (glm::mat4(*)( glm::mat4 const&, glm::vec3 const& )) &::glm::translate ),
                luabind::def( "scale",       (glm::mat4(*)( glm::mat4 const&, glm::vec3 const& )) &::glm::scale ),
                luabind::def( "rotate",      (glm::mat4(*)( glm::mat4 const&, float const&, glm::vec3 const& )) &::glm::rotate ),
                luabind::def( "inverse",     (glm::mat4(*)( glm::mat4 const& )) &::glm::inverse ),
                luabind::def( "transpose",   (glm::mat4(*)( glm::mat4 const& )) &::glm::transpose ),
                luabind::def( "transpose",   (glm::mat3(*)( glm::mat3 const& )) &::glm::transpose ),
                luabind::def( "inverseTranspose",   (glm::mat3(*)( glm::mat3 const& )) &::glm::inverseTranspose ),

                luabind::def( "inverse",    (glm::mat3(*)( glm::mat3 const& )) &::glm::inverse ),
                luabind::def( "transpose",  (glm::mat3(*)( glm::mat3 const& )) &::glm::transpose ),
				luabind::def( "project",    (glm::vec3(*)( glm::vec3 const&, glm::mat4 const&, glm::mat4 const&, glm::vec4 const&)) &::glm::project ),
				luabind::def( "unProject",  (glm::vec3(*)( glm::vec3 const&, glm::mat4 const&, glm::mat4 const&, glm::vec4 const&)) &::glm::unProject ),

                luabind::def( "rotateQuat", (glm::quat(*)( glm::quat const&, float const&, glm::vec3 const& )) &::glm::rotate ),
                luabind::def( "normalise",  (glm::quat(*)( glm::quat const& )) &::glm::normalize ),
                luabind::def( "cross",      (glm::quat(*)( glm::quat const&, glm::quat const& )) &::glm::cross ),
                luabind::def( "mat4Cast",   (glm::mat4(*)( glm::quat const& )) &::glm::mat4_cast ),
                luabind::def( "squad",		(glm::quat(*)( glm::quat const&, glm::quat const&, glm::quat const&, glm::quat const&, float const& )) &glm::squad ),
				luabind::def( "intermediate",(glm::quat(*)( glm::quat const&, glm::quat const&, glm::quat const& )) &glm::intermediate )
            ];
        }
	}
}