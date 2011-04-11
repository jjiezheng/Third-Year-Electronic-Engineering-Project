#include "luaSound.h"


#include "luaSystem.h"
#include "Music.h"
#include "SoundEffect.h"

namespace de
{
    namespace luaInterface
    {
        void luaSound( lua_State *_luaState )
		{
			luabind::module(_luaState )
            [
                luabind::class_< de::music::Music >( "music" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor<const std::string &>() )
					.def( "play", (void(de::music::Music::*)())  &de::music::Music::play )
                    .def( "load", (void(de::music::Music::*)(std::string const&))  &de::music::Music::load ),

				luabind::class_< de::music::SoundEffect >( "sound_effect" )
                    .def( luabind::constructor<>() )
                    .def( luabind::constructor<const std::string &>() )
					.def( "play", (void(de::music::SoundEffect::*)())  &de::music::SoundEffect::play )
                    .def( "load", (void(de::music::SoundEffect::*)(std::string const&))  &de::music::SoundEffect::load )
            ];
		}
    }
}
