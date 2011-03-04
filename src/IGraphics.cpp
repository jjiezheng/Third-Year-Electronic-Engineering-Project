#include "IGraphics.h"
#include "CoreEnumsAndClasses.h"

namespace de
{
    namespace graphics
    {

        NullGraphics::NullGraphics()
        {
            Display = NULL;
            //Display = SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF );
            //SDL_SetVideoMode( 640, 480, 32, SDL_OPENGL | SDL_RESIZABLE  );
        }

        frameDetails NullGraphics::render()
        {
            static frameDetails details;

            //SDL_Flip(Display);
            //CHECKGL( SDL_GL_SwapBuffers() );
            return details;
        }
    }
}
