#ifndef OVERLAY_H
#define OVERLAY_H

#include "State.h"


#include "CoreEngine.h"

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
			private:
				void reLoadTextures();
				void loadLuaState();

				bool active;
				State *state;

				unsigned int totalTime;

				std::ofstream benchmarkFile;
		};
	}
}
#endif // OVERLAY_H
