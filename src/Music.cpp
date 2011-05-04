#include "Music.h"
#include "GameServices.h"


namespace de
{
	namespace music
	{
		Music::Music( const std::string &_file) : musicHandle(0)
		{
			load( _file );
		}

		Music::Music() : musicHandle(0)
		{
		}

		Music::~Music()
		{

		}


		void Music::load( const std::string &_file )
		{
			musicHandle = de::Engine::Resources().getMusic( _file );
		}

		void Music::play()
		{
			de::Engine::Audio().playMusic( musicHandle );
		}
	}
}