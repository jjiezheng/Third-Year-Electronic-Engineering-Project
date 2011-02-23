#include "Loader.h"

using namespace de::classes;
using namespace de::enums;




namespace de
{
	namespace state
	{
		Loader::Loader()
			:first(true)
		{

		}

		Loader::~Loader()
		{
			//dtor
		}



		bool Loader::handleEvents( const SDL_Event &_event )
		{
			return true;
		}


		bool Loader::logic( const Uint32 &_deltaticks, State* &_nextState, state::options &_options )
		{
			if( first )
			{
				loading.set( "Loading", "visitor", Vector( 640, 20 ), 16, ALIGN_CENTRE );
			}
			return true;
		}


		void Loader::render()
		{
			loading.render( FBO_AFTER );
		}



		void Loader::reLoadTextures()
		{
			loading.reload();
		}

	}
}