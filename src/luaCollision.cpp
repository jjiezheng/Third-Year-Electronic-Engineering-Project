#include "luaCollision.h"
#include "VisualHitbox.h"
#include "OBB.h"

#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>

namespace de
{
    namespace luaInterface
    {
        void exportCollision( lua_State *_luaState )
		{
			luabind::module(_luaState, "collision" )
            [
                luabind::class_<de::collision::OBB>( "obb" )
                    .def( luabind::constructor<>() )
					.def( luabind::constructor<const float &, const float &>() )
                    .def( "transform", (void(de::collision::OBB::*)( const glm::vec2 & )) &de::collision::OBB::transform )
                    .def( "teleport",  (void(de::collision::OBB::*)( const glm::vec2 & )) &de::collision::OBB::teleport )
					.def( "rotate",  (void(de::collision::OBB::*)( const float & )) &de::collision::OBB::rotate )
					.def( "rotate",  (void(de::collision::OBB::*)( const float &, const float & )) &de::collision::OBB::rotate )
					.def( "contains_point",  (bool(de::collision::OBB::*)( const glm::vec3 & )) &de::collision::OBB::containsPoint ),


                luabind::class_<de::collision::VisualHitbox>( "visualHitbox" )
                    .def( luabind::constructor<>() )
					.def( luabind::constructor<const de::collision::OBB &>() )
                    .def( "update", (void(de::collision::VisualHitbox::*)( const de::collision::OBB & )) &de::collision::VisualHitbox::update )
                    .def( "reload",  (void(de::collision::VisualHitbox::*)())	&de::collision::VisualHitbox::reload )
					.def( "render",  (void(de::collision::VisualHitbox::*)())	&de::collision::VisualHitbox::render )

					.def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, float const& ))		&de::collision::VisualHitbox::setUniform )
                    .def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, glm::vec2 const& ))	&de::collision::VisualHitbox::setUniform )
                    .def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, glm::vec3 const& ))	&de::collision::VisualHitbox::setUniform )
                    .def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, glm::vec4 const& ))	&de::collision::VisualHitbox::setUniform )
                    .def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, glm::mat3 const& ))	&de::collision::VisualHitbox::setUniform )
                    .def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, glm::mat4 const& ))	&de::collision::VisualHitbox::setUniform )
                    .def( "uniform",  (void(de::collision::VisualHitbox::*)( std::string const&, std::string const& ))	&de::collision::VisualHitbox::setUniform )
			];

		}
    }
}