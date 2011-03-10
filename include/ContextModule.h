#ifndef CONTEXTMODULE_H
#define CONTEXTMODULE_H

#include "frameDetails.h"

namespace de
{
	namespace graphics
	{
		class ContextModule
		{
			public:
				/** Default constructor */
				ContextModule();
				/** Default destructor */
				virtual ~ContextModule();

				bool create( VideoInfo &_videoSettings );
				bool AA();
				int width();
				int height();
			protected:
			private:
				void setOpenGLContext( int _width, int _height, int _aa, int _colourDepth, int _flags );
				void getVersion( VideoInfo &_videoSettings );
				
				int screenWidth, screenHeight;
				bool multiSample;
		};
	}
}
#endif // CONTEXTMODULE_H
