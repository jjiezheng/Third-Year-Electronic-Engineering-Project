#ifndef LOADER_H
#define LOADER_H

#include "State.h"

#include "CoreEngine.h"
#include "Coords.h"
#include "ColourBlender.h"
#include "UIbaseData.h"


#include <iostream>
#include <fstream>


namespace de
{
	namespace state
	{
		class Loader : public State
		{
			public:
				Loader();
				virtual ~Loader();

				bool handleEvents( const SDL_Event &_event );
				bool logic( const Uint32 &_deltaticks, State* &_nextState, state::options &_options );
				void render();

			private:
				void reLoadTextures();

			private:
				bool first;
				de::classes::SpriteString loading;

		};
	}
}

#endif // LOADER_H
