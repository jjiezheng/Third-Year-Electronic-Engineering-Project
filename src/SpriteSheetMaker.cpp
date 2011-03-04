#include "SpriteSheetMaker.h"
#include <luabind/luabind.hpp>


using namespace de::classes;
using namespace de::core;
using namespace de::enums;
using namespace de::misc;
using namespace de::internal;
using namespace de::filesystem;
using namespace de::graphics;

namespace fs = boost::filesystem;

namespace de
{
	namespace state
			{
		SpriteSheetMaker::SpriteSheetMaker()
		{
			luaState = lua_open();
			luabind::open(luaState);


			luabind::module(luaState)
			[
				luabind::class_<SpriteSheet>("SpriteSheet")
				.def(luabind::constructor<const std::string, const std::string, int, int>())
			];

			std::string path( Roots->get( root::TEXTURES ) +  "../images/SpriteSheets.lua");
			luaL_dofile(luaState, path.c_str() );

		}

		SpriteSheetMaker::~SpriteSheetMaker()
		{
			lua_close(luaState);
		}

		bool SpriteSheetMaker::handleEvents( const SDL_Event &_event )
		{
			return false;
		}

		bool SpriteSheetMaker::logic( const Uint32 &_deltaTicks, State* &_nextState, options &_options )
		{
			return false;
		}

		void SpriteSheetMaker::render()
		{
		}

		void SpriteSheetMaker::reLoadTextures()
		{
		}

	}
}