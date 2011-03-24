#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "Base.h"

namespace de
{
	namespace classes
	{
		class Frect;
	}
    namespace resources
    {
		class ImageResource
        {
            public:
                //! Texture width and Height.
                int width, height;

                //! A handle to the actual Texture.
                Uint32 Texture;

                ImageResource() : width(0), height(0) {}
                ImageResource( const std::string &_name, const boost::filesystem::path &_path );
				~ImageResource();

                //! Get the textures handle
                const Uint32& getTexture( lua_State* L = NULL );
                //! Unloads the Texture
                void unload();
                //! Loads the Texture
                void load( lua_State* L = NULL );

                classes::Frect getSpriteCoords( const std::string _name, bool tex = true );
                std::map<std::string, classes::Frect> getAllSprites( bool tex = true );

                std::vector<std::string> getSpriteNames();
            private:


                std::string name;
                boost::filesystem::path path, luaPath;

                std::vector<std::string> spriteNames;
                std::map<std::string, classes::Frect> spriteRects, spriteTexs;
                std::map<std::string, classes::Frect>::iterator frectIter;
        };
	}
}
#endif //IMAGERESOURCE_H

