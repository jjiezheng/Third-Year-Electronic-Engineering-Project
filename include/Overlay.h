#ifndef OVERLAY_H
#define OVERLAY_H

#include "State.h"
#include "LuaState.h"

#include "CoreEngine.h"
#include "Coords.h"
#include "Entities.h"
#include "Grapher.h"
#include "ColourBlender.h"
#include "UIbaseData.h"


#include <iostream>
#include <fstream>


namespace de
{
	namespace state
	{
		class Overlay : public State
		{
			public:
				Overlay();
				virtual ~Overlay();

				bool handleEvents( const SDL_Event &_event );
				bool logic( const Uint32 &_deltaticks, State* &_nextState, options &_options );
				void render();


				void graph( unsigned int _time/*, de::enums::de_GraphItems _itemType*/ );
				void endOfFrame();
				void newState();
			private:
				void reLoadTextures();
				void loadLuaState();

				bool active;
				LuaState state;

				Grapher frameRateGraph, spriteGraph, batchGraph, timeGraph;
				de::classes::SpriteString message;

				unsigned int totalTime, explosionTime;
				bool luaError;

				std::ofstream benchmarkFile;
		};
	}
}
#endif // OVERLAY_H
