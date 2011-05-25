#include "luaHaptics.h"
#include "Haptics.h"

#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>

namespace de
{
    namespace haptics
    {
        class dummyClass
        {
            public:
                dummyClass(){}
                ~dummyClass(){}
        };
	}
}

namespace de
{
    namespace luaInterface
    {
        void exportHaptics( lua_State *_luaState )
		{
			using namespace de::classes;
			luabind::module(_luaState, "haptics" )
			[
				luabind::def( "workspace", (void(*)(const glm::mat4 &, const glm::mat4 &)) &::de::haptics::workspace ),
				luabind::def( "run", (void(*)()) &::de::haptics::runFrame ),
				luabind::def( "start_effect", (int(*)(const std::string &, const glm::vec3 &, const glm::vec3 &, const double &, const double &)) &::de::haptics::startEffect ),
				luabind::def( "stop_effect", (void(*)(const int &)) &::de::haptics::stopEffect ),
				luabind::def( "stop_all_effects", (void(*)()) &::de::haptics::stopAllEffects ),

				luabind::def( "proxy_position", (glm::vec3 (*)()) &::de::haptics::getProxy ),
				luabind::def( "proxy_transform", (glm::mat4 (*)()) &::de::haptics::getProxyTransform )
			];
		}
    }
}