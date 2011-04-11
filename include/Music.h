#ifndef MUSIC_H
#define MUSIC_H

#include "Base.h"
namespace de
{
	namespace music
	{
		class Music
		{
			public:
				Music( const std::string &_file );
				Music();
				~Music(void);

				void load( const std::string &_file );
				void play();
			private:
				int musicHandle;
		};
	}
}


#endif //MUSIC_H
