#ifndef SPRITESHEETMAKER_H
#define SPRITESHEETMAKER_H

#include "State.h"
#include "CoreEngine.h"
#include "Coords.h"

#include "SpriteSheet.h"

#include "Entities.h"
#include "ColourBlender.h"

namespace de
{
	namespace state
	{
		class SpriteSheetMaker : public State
		{
			public:
				SpriteSheetMaker();
				~SpriteSheetMaker();

				bool handleEvents( const SDL_Event &_event );
				bool logic( const Uint32 &_deltaTicks, State* &_nextState, options &_options );
				void render();

			private:
				void reLoadTextures();

				lua_State *luaState;
		};
	}
}

#endif // SPRITESHEETMAKER_H
