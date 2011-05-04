#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H

#include "Base.h"
namespace de
{
	namespace music
	{
		class SoundEffect
		{
			public:
				SoundEffect( const std::string &_file );
				SoundEffect();
				~SoundEffect(void);

				void load( const std::string &_file );
				void play();
			private:
				int handle;
		};
	}
}


#endif //SOUND_EFFECT_H
