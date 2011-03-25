#include "Overlay.h"

#include "LuaState.h"
#include "LuaInterface.h"
#include <boost/ref.hpp>

using namespace de::classes;
using namespace de::enums;
using namespace de::filesystem;

namespace fs = boost::filesystem;

namespace de
{
	namespace state
	{

		Overlay::Overlay() :
			active(false),
			totalTime(0)
		{
			benchmarkFile.open ("../benchmark.csv");
			benchmarkFile << "Control,Logic,Render,Rendering,idleTime,TotalTime\n";

			state = new LuaState( "Overlay", true );
		}

		Overlay::~Overlay()
		{
			benchmarkFile.close();
		}

		bool Overlay::handleEvents( const SDL_Event &_event )
		{
			using namespace de::events;
			state->handleEvents( _event );

			if( isKeyDown( _event, SDLK_LCTRL ) && wasKeyPressed( _event, SDLK_TAB ) )
			{
				active = !active;
			}
			if(  wasKeyPressed( _event, SDLK_F7 ) || wasKeyPressed( _event, SDLK_F8 ) )
			{
				active = false;
			}

			if( _event.type == SDL_USEREVENT )
			{
				if( _event.user.code == de::enums::events::OPENGL_RELOAD )
				{
					reLoadTextures();
				}
				return true;
			}
			return !active;
		}

		bool Overlay::logic( const Uint32 &_deltaticks, State* &_nextState, options &_options )
		{
			state->logic( _deltaticks, _nextState, _options );

			//return !luabind::object_cast<bool>( currentState[ "isActive" ] );
			return !active;
		}

		void Overlay::render()
		{
			state->render();
		}

		void Overlay::reLoadTextures()
		{
		}

		void Overlay::graph( unsigned int _time/*, de_GraphItems _itemType*/ )
		{
			benchmarkFile << _time << ",";

			totalTime += _time;
		}

		void Overlay::endOfFrame()
		{
			benchmarkFile << totalTime << "\n";
			totalTime = 0;
		}
	}
}
