#ifndef TEXTUREMODULE_H
#define TEXTUREMODULE_H

#include "Base.h"

namespace de
{
    namespace graphics
    {
        class TextureModule
        {
            public:
                /** Default constructor */
                TextureModule();
                /** Default destructor */
                virtual ~TextureModule();

                Uint32 load( const std::string& _fileName, int &_width, int &_height );
                Uint32 load( const std::string& _fileName, int &_width, int &_height, int _textureDimensions );

                bool unload( Uint32 &_texture );
            protected:
            private:
        };
    }
}

#endif // TEXTUREMODULE_H
