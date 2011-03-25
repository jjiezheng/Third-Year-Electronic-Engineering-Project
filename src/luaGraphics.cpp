#include "luaGraphics.h"

#include "Object3D.h"
#include "SpriteInstance.h"
#include "SpriteObject.h"
#include "StringObject.h"
#include "modelObject.h"


#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/inverse_transpose.hpp>


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
        void export2D( lua_State *_luaState )
        {
        }

        void export3D( lua_State *_luaState )
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
                    .def( luabind::self-glm::vec3() )
                    .def( luabind::self+glm::vec3() ),

                luabind::class_<glm::vec4>( "vec4" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor< const float& >() )
                    .def( luabind::constructor< const float&, const float&, const float&, const float& >() )
                    .def( luabind::constructor< const glm::vec3&, const float& >() )
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



                luabind::def( "rotateQuat", (glm::quat(*)( glm::quat const&, float const&, glm::vec3 const& )) &::glm::rotate ),
                luabind::def( "normalise",  (glm::quat(*)( glm::quat const& )) &::glm::normalize ),
                luabind::def( "cross",      (glm::quat(*)( glm::quat const&, glm::quat const& )) &::glm::cross ),
                luabind::def( "mat4Cast",   (glm::mat4(*)( glm::quat const& )) &::glm::mat4_cast ),


                 luabind::class_< std::vector<int> >( "intArray" )
                    .def( luabind::constructor<>() )
                    .def( "clear", (void(std::vector<int>::*)())  &std::vector<int>::clear )
                    .def( "add",   (void(std::vector<int>::*)( const int& ))  &std::vector<int>::push_back ),

                 luabind::class_< std::vector<de::graphics::vertex> >( "vertexArray" )
                    .def( luabind::constructor<>() )
                    .def( "clear", (void(std::vector<de::graphics::vertex>::*)())  &std::vector<de::graphics::vertex>::clear )
                    .def( "add",   (void(std::vector<de::graphics::vertex>::*)( const de::graphics::vertex& ))  &std::vector<de::graphics::vertex>::push_back ),


                luabind::class_<Object3D>( "mesh" )
                    .def( luabind::constructor<>() )
                    .def( "uniform", (Object3D&(Object3D::*)( std::string const&, float const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    //.def( "setUniform", (Object3D&(Object3D::*)( std::string const&, int const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (Object3D&(Object3D::*)( std::string const&, glm::vec2 const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (Object3D&(Object3D::*)( std::string const&, glm::vec3 const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (Object3D&(Object3D::*)( std::string const&, glm::vec4 const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (Object3D&(Object3D::*)( std::string const&, glm::mat3 const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (Object3D&(Object3D::*)( std::string const&, glm::mat4 const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (Object3D&(Object3D::*)( std::string const&, std::string const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )

                    .def( "depth",    (Object3D&(Object3D::*)( bool )) &Object3D::depth, luabind::return_reference_to(_1) )
					.def( "writeToDepth",(Object3D&(Object3D::*)( bool )) &Object3D::writeToDepth, luabind::return_reference_to(_1) )
					.def( "alpha",    (Object3D&(Object3D::*)( bool )) &Object3D::alpha, luabind::return_reference_to(_1) )
					.def( "blend",    (Object3D&(Object3D::*)( bool )) &Object3D::blend, luabind::return_reference_to(_1) )
					

                    .def( "shader",   (Object3D&(Object3D::*)( std::string const&)) &Object3D::set, luabind::return_reference_to(_1) )
                    .def( "vert_type",(Object3D&(Object3D::*)( std::string const&)) &Object3D::setType, luabind::return_reference_to(_1) )
                    .def( "add",      (Object3D&(Object3D::*)( std::string const&, std::vector<de::graphics::vertex> const& )) &Object3D::add, luabind::return_reference_to(_1) )
                    .def( "add",      (Object3D&(Object3D::*)( std::string const&, std::vector<de::graphics::vertex> const&, std::vector<int> const& )) &Object3D::add, luabind::return_reference_to(_1) )
                    .def( "load",   (void(Object3D::*)( std::string const&, std::string const&)) &Object3D::load )
                    .def( "reload",   (void(Object3D::*)()) &Object3D::reload )
                    .def( "render",   (void(Object3D::*)()) &Object3D::render ),


					
                luabind::class_<modelObject>( "model" )
                    .def( luabind::constructor<>() )
                    .def( "uniform",	(modelObject&(modelObject::*)( std::string const&, float const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
                    //.def( "uniform",	(modelObject&(modelObject::*)( std::string const&, int const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",	(modelObject&(modelObject::*)( std::string const&, glm::vec2 const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",	(modelObject&(modelObject::*)( std::string const&, glm::vec3 const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",	(modelObject&(modelObject::*)( std::string const&, glm::vec4 const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",	(modelObject&(modelObject::*)( std::string const&, glm::mat3 const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",	(modelObject&(modelObject::*)( std::string const&, glm::mat4 const& )) &modelObject::setUniform, luabind::return_reference_to(_1) )
					.def( "texture",	(modelObject&(modelObject::*)( std::string const&, std::string const&, int )) &modelObject::setTexture, luabind::return_reference_to(_1) )

                    .def( "depth",		(modelObject&(modelObject::*)( bool )) &modelObject::depth, luabind::return_reference_to(_1) )
					.def( "writeToDepth",(modelObject&(modelObject::*)( bool ))&modelObject::writeToDepth, luabind::return_reference_to(_1) )
					.def( "alpha",		(modelObject&(modelObject::*)( bool )) &modelObject::alpha, luabind::return_reference_to(_1) )
					.def( "blend",		(modelObject&(modelObject::*)( bool )) &modelObject::blend, luabind::return_reference_to(_1) )
					.def( "ccw",		(modelObject&(modelObject::*)( bool )) &modelObject::ccw, luabind::return_reference_to(_1) )

                    .def( "type",		(modelObject&(modelObject::*)( std::string const&)) &modelObject::setType, luabind::return_reference_to(_1) )
                    .def( "load",		(void(modelObject::*)( std::string const&, std::string const&)) &modelObject::load )
                    .def( "reload",		(void(modelObject::*)()) &modelObject::reload )
                    .def( "render",		(void(modelObject::*)()) &modelObject::render ),
					

                luabind::class_<StringObject>( "stringObject" )
                    .def( luabind::constructor<>() )
                    .def( "uniform", (StringObject&(StringObject::*)( std::string const&, float const& )) &StringObject::setUniform, luabind::return_reference_to(_1) )
                    //.def( "setUniform", (Object3D&(Object3D::*)( std::string const&, int const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (StringObject&(StringObject::*)( std::string const&, glm::vec2 const& )) &StringObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (StringObject&(StringObject::*)( std::string const&, glm::vec3 const& )) &StringObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (StringObject&(StringObject::*)( std::string const&, glm::vec4 const& )) &StringObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (StringObject&(StringObject::*)( std::string const&, glm::mat3 const& )) &StringObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (StringObject&(StringObject::*)( std::string const&, glm::mat4 const& )) &StringObject::setUniform, luabind::return_reference_to(_1) )

                    .def( "depth",    (StringObject&(StringObject::*)( bool )) &StringObject::depth, luabind::return_reference_to(_1) )
					.def( "writeToDepth",(StringObject&(StringObject::*)( bool )) &StringObject::writeToDepth, luabind::return_reference_to(_1) )
					.def( "alpha",    (StringObject&(StringObject::*)( bool )) &StringObject::alpha, luabind::return_reference_to(_1) )
					.def( "blend",    (StringObject&(StringObject::*)( bool )) &StringObject::blend, luabind::return_reference_to(_1) )

                    .def( "font",    (StringObject&(StringObject::*)( std::string const& )) &StringObject::font, luabind::return_reference_to(_1) )
					.def( "text",    (StringObject&(StringObject::*)( std::string const& )) &StringObject::text, luabind::return_reference_to(_1) )
					.def( "shader",  (StringObject&(StringObject::*)( std::string const& )) &StringObject::shader, luabind::return_reference_to(_1) )

					.def( "align",   (glm::vec3&(StringObject::*)()) &StringObject::align )

                    .def( "reload",   (void(StringObject::*)()) &StringObject::reload )
                    .def( "render",   (void(StringObject::*)()) &StringObject::render ),

                luabind::class_<SpriteObject>( "Sprite3D" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor<std::string const&, std::string const&, std::string const&>() )
                    .def( "uniform", (SpriteObject&(SpriteObject::*)( std::string const&, float const& )) &SpriteObject::setUniform, luabind::return_reference_to(_1) )
                    //.def( "setUniform", (Object3D&(Object3D::*)( std::string const&, int const& )) &Object3D::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (SpriteObject&(SpriteObject::*)( std::string const&, glm::vec2 const& )) &SpriteObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (SpriteObject&(SpriteObject::*)( std::string const&, glm::vec3 const& )) &SpriteObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (SpriteObject&(SpriteObject::*)( std::string const&, glm::vec4 const& )) &SpriteObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (SpriteObject&(SpriteObject::*)( std::string const&, glm::mat3 const& )) &SpriteObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (SpriteObject&(SpriteObject::*)( std::string const&, glm::mat4 const& )) &SpriteObject::setUniform, luabind::return_reference_to(_1) )
                    .def( "uniform",  (SpriteObject&(SpriteObject::*)( std::string const&,std::string const&)) &SpriteObject::setUniform, luabind::return_reference_to(_1) )

                    .def( "set",   (SpriteObject&(SpriteObject::*)( std::string const&,std::string const&,std::string const& )) &SpriteObject::set, luabind::return_reference_to(_1) )
                    .def( "addSprite",   (SpriteObject&(SpriteObject::*)( std::string const&, std::string const&, int ))          &SpriteObject::addSprite, luabind::return_reference_to(_1) )


                    .def( "reload",   (void(SpriteObject::*)()) &SpriteObject::reload )
                    .def( "render",   (void(SpriteObject::*)()) &SpriteObject::render ),

                luabind::class_<SpriteInstance>( "Instance" )
                    .def( luabind::constructor<>() )
                    .def( "seed", (void(SpriteInstance::*)(const std::string&,const std::string &)) &SpriteInstance::seed )
                    .def( "add", (void(SpriteInstance::*)(OpaqueSprite*,int)) &SpriteInstance::add )
                    .def( "getId", (void(SpriteInstance::*)(const std::string&,const std::string&)) &SpriteInstance::getId )
                    .def( "reload", (void(SpriteInstance::*)()) &SpriteInstance::reload )
                    .def( "render", (void(SpriteInstance::*)()) &SpriteInstance::render ),

                luabind::class_<OpaqueSprite>( "OpaqueSprite" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor<SpriteInstance*, const std::string&, const std::string&>() )
                    .def( "id",           (int(OpaqueSprite::*)())                  &OpaqueSprite::id )
                    .def( "addThisFrame", (void(OpaqueSprite::*)())                 &OpaqueSprite::addThisFrame )
                    .def( "change",       (void(OpaqueSprite::*)(const std::string&, int))&OpaqueSprite::change )
                    .def( "rotation",     (void(OpaqueSprite::*)(const float&))     &OpaqueSprite::setRotation )
                    .def( "position",     (void(OpaqueSprite::*)(const glm::vec3&)) &OpaqueSprite::setPosition )
                    .enum_("constants")
                    [
                        luabind::value("NONE", OpaqueSprite::NONE ),
                        luabind::value("SPRITE", OpaqueSprite::SPRITE ),
                        luabind::value("SHADER", OpaqueSprite::SHADER )
                    ]

            ];

        }

	}
}