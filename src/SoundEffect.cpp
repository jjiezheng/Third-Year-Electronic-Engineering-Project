#include "SoundEffect.h"
#include "GameServices.h"

namespace de
{
	namespace music
	{
		SoundEffect::SoundEffect( const std::string &_file ) : handle(0)
		{
			load( _file );
		}

		SoundEffect::SoundEffect() : handle(0)
		{
		}

		SoundEffect::~SoundEffect()
		{

		}


		void SoundEffect::load( const std::string &_file )
		{
			handle = de::Engine::Resources().getSoundEffect( _file );
		}

		void SoundEffect::play()
		{
			de::Engine::Audio().playSoundEffect( handle );
		}
	}
}