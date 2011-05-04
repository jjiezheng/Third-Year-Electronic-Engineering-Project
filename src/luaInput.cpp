#include "luaInput.h"

namespace de
{
    namespace luaInterface
    {
		void exportInput( lua_State *_luaState )
		{
			exportKeyBoard( _luaState );
			exportMouse( _luaState );
			exportSDLKeys( _luaState );
		}

        void exportKeyBoard( lua_State *_luaState )
        {
            using namespace de::classes;

            luabind::module(_luaState, "events" )
            [
                luabind::class_<SDL_Event>( "SDL_Event" )
                    .def( luabind::constructor<>() ),
                luabind::class_<Uint16>( "Uint16" )
                    .def( luabind::constructor<>() ),

                luabind::def( "wasKeyPressedUnicode", (bool(*)(const SDL_Event &, const std::string &)) &::de::events::wasKeyPressedUnicode ),
                luabind::def( "wasKeyPressed",        (bool(*)(const SDL_Event &, const int &)) &::de::events::wasKeyPressed ),
                luabind::def( "getKeyPressed",        (std::string(*)(const SDL_Event &)) &::de::events::getKeyPressed ),
                //luabind::def( "isEvent",            (bool(*)(const SDL_Event &, const int &)) &::de::events::isEvent ),
                luabind::def( "isButtonDown",         (bool(*)(const SDL_Event &, const int &)) &::de::events::isMouseButtonDown ),
                luabind::def( "isButtonUp",           (bool(*)(const SDL_Event &, const int &)) &::de::events::isMouseButtonUp ),
                luabind::def( "isKeyDown",            (bool(*)(const SDL_Event &, const int &)) &::de::events::isKeyDown ),
				luabind::def( "get_key",              (float(*)(const SDL_Event &)) &::de::events::getkey )
            ];
        }        
		
		void exportMouse( lua_State *_luaState )
        {
            using namespace de::classes;

            luabind::module(_luaState, "events" )
            [

                luabind::class_<SDL_Event>( "SDL_Event" )
                    .def( luabind::constructor<>() ),
                luabind::class_<Uint16>( "Uint16" )
                    .def( luabind::constructor<>() ),

                luabind::def( "relativeMouseMovement",(glm::vec3(*)(const SDL_Event & )) &::de::events::getRelativeMouseMovementVec3 ),
                luabind::def( "relativeMouseAngle",   (float(*)(const SDL_Event &, int _axis )) &::de::events::relativeMouseAngle ),
	            luabind::def( "mouse_position",       (glm::vec3(*)(const SDL_Event & )) &::de::events::MousePosition ),
                luabind::def( "showMouse",            (void(*)(bool)) &::de::events::showMouse ),
                luabind::def( "toggleMouse",          (void(*)()) &::de::events::toggleMouse ),
                luabind::def( "toggleMouseGrab",      (void(*)()) &::de::events::toggleMouseGrab ),
                luabind::def( "resetMouse",           (void(*)()) &::de::events::resetMouse )
            ];
        }

        void exportSDLKeys( lua_State *_luaState )
        {
            luabind::module(_luaState )
            [
                luabind::class_<de::events::dummyClassSDLKey>( "mouse_event_type" )
                    .def( luabind::constructor<>() )
                    .enum_("constants")
                    [
                        luabind::value("mouseDown", SDL_MOUSEBUTTONDOWN),
                        luabind::value("mouseUp", SDL_MOUSEBUTTONUP)
                    ],

                    luabind::class_<de::events::dummyClassSDLKey>( "mouse" )
                    .def( luabind::constructor<>() )
                    .enum_("constants")
                    [
                        luabind::value("left", SDL_BUTTON_LEFT),
                        luabind::value("right", SDL_BUTTON_RIGHT),
                        luabind::value("middle", SDL_BUTTON_MIDDLE),
                        luabind::value("wheelup", SDL_BUTTON_WHEELUP),
                        luabind::value("wheeldown", SDL_BUTTON_WHEELDOWN)
                    ],

                luabind::class_<de::events::dummyClassSDLKey>( "key" )
                    .def( luabind::constructor<>() )
                    .enum_("constants")
                    [
                        luabind::value("0", SDLK_0),
                        luabind::value("1", SDLK_1),
                        luabind::value("2", SDLK_2),
                        luabind::value("3", SDLK_3),
                        luabind::value("4", SDLK_4),
                        luabind::value("5", SDLK_5),
                        luabind::value("6", SDLK_6),
                        luabind::value("7", SDLK_7),
                        luabind::value("8", SDLK_8),
                        luabind::value("9", SDLK_9),
                        luabind::value("-", SDLK_MINUS),
                        luabind::value("+", SDLK_PLUS),
                        luabind::value("[", SDLK_LEFTBRACKET),
                        luabind::value("]", SDLK_RIGHTBRACKET),
                        luabind::value("a", SDLK_a),
                        luabind::value("b", SDLK_b),
                        luabind::value("c", SDLK_c),
                        luabind::value("d", SDLK_d),
                        luabind::value("e", SDLK_e),
                        luabind::value("f", SDLK_f),
                        luabind::value("g", SDLK_g),
                        luabind::value("h", SDLK_h),
                        luabind::value("i", SDLK_i),
                        luabind::value("j", SDLK_j),
                        luabind::value("k", SDLK_k),
                        luabind::value("l", SDLK_l),
                        luabind::value("m", SDLK_m),
                        luabind::value("n", SDLK_n),
                        luabind::value("o", SDLK_o),
                        luabind::value("p", SDLK_p),
                        luabind::value("q", SDLK_q),
                        luabind::value("r", SDLK_r),
                        luabind::value("s", SDLK_s),
                        luabind::value("t", SDLK_t),
                        luabind::value("u", SDLK_u),
                        luabind::value("v", SDLK_v),
                        luabind::value("w", SDLK_w),
                        luabind::value("x", SDLK_x),
                        luabind::value("y", SDLK_y),
                        luabind::value("z", SDLK_z),
                        luabind::value("f1", SDLK_F1),
                        luabind::value("f2", SDLK_F2),
                        luabind::value("f3", SDLK_F3),
                        luabind::value("f4", SDLK_F4),
                        luabind::value("f5", SDLK_F5),
                        luabind::value("f6", SDLK_F6),
                        luabind::value("f7", SDLK_F7),
                        luabind::value("f8", SDLK_F8),
                        luabind::value("f9", SDLK_F9),
                        luabind::value("f10", SDLK_F10),
                        luabind::value("f11", SDLK_F11),
                        luabind::value("f12", SDLK_F12),
                        luabind::value("tab", SDLK_TAB),
                        luabind::value("crtl", SDLK_LCTRL),
                        luabind::value("lctrl", SDLK_LCTRL),
                        luabind::value("rctrl", SDLK_RCTRL),
                        luabind::value("shift", SDLK_LSHIFT ),
                        luabind::value("lshift", SDLK_LSHIFT ),
                        luabind::value("rshift", SDLK_RSHIFT ),
                        luabind::value("space", SDLK_SPACE ),
                        luabind::value("escape", SDLK_ESCAPE ),
                        luabind::value("home", SDLK_HOME ),
                        luabind::value("end", SDLK_END ),
                        luabind::value("up", SDLK_UP ),
						luabind::value("down", SDLK_DOWN ),
						luabind::value("left", SDLK_LEFT ),
						luabind::value("right", SDLK_RIGHT ),
						luabind::value("enter", SDLK_RETURN ),
						luabind::value("return", SDLK_RETURN ),
						luabind::value("backspace", SDLK_BACKSPACE )
                    ]

            ];
        }
	}
}