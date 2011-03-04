#include "TextureModule.h"

#include "openGL.h"
#include <corona.h>

namespace de
{
    namespace graphics
    {
        TextureModule::TextureModule()
        {
            //ctor
        }

        TextureModule::~TextureModule()
        {
            //dtor
        }


        Uint32 TextureModule::load( const std::string& _fileName, int &_width, int &_height )
        {
            return load( _fileName, _width, _height, GL_TEXTURE_2D );
        }

        Uint32 TextureModule::load( const std::string& _fileName, int &_width, int &_height, int _textureDimensions )
        {
            corona::Image* image = corona::OpenImage( _fileName.c_str(), corona::PF_R8G8B8A8 );
            if (!image)
            {
                de::io::error << "Failed to load image " << _fileName << "\n";
                delete image;
                return 0;
            }

            int width = _width = image->getWidth();
            int height = _height = image->getHeight();

            GLuint textureid;
            CHECKGL( glGenTextures( 1, &textureid ) );
            CHECKGL( glBindTexture( _textureDimensions, textureid ) );


            if( _textureDimensions == GL_TEXTURE_1D )
                glTexImage1D( _textureDimensions, 0, GL_RGBA,  width, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixels() );

            else if( _textureDimensions == GL_TEXTURE_2D )
                glTexImage2D( _textureDimensions, 0, GL_RGBA,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixels() );

            else if( _textureDimensions == GL_TEXTURE_3D )
            {
                de::io::error << "Error loading texture at " << _fileName << ". 3D Textures not supported yet.\n";
                return 0;
            }
            else
            {
                de::io::error << "Error loading texture at " << _fileName << " Due to bad GLenum.\n";
                return 0;
            }
            CHECKGL( glGenerateMipmap(_textureDimensions) );

            CHECKGL( glTexParameteri( _textureDimensions,GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
            CHECKGL( glTexParameterf( _textureDimensions, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );

            // the texture wrade over at the edges (repeat)
            //CHECKGL( glTexParameterf( _textureDimensions, GL_TEXTURE_WRAP_S, GL_REPEAT ) );
            //CHECKGL( glTexParameterf( _textureDimensions, GL_TEXTURE_WRAP_T, GL_REPEAT ) );

            CHECKGL( glTexParameterf( _textureDimensions, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            CHECKGL( glTexParameterf( _textureDimensions, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
            delete image;

            return textureid;
        }


        bool TextureModule::unload( Uint32 &_texture )
        {
            if( _texture != 0 )
            {
                CHECKGL( glDeleteTextures( 1, &_texture ) );
                _texture = 0;
                return true;
            }
            return false;
        }
    }
}
