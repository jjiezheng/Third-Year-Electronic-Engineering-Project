#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "State.h"
#include "CoreEngine.h"
#include "Coords.h"

#include "FrameBufferObject.h"
#include "SpriteBuffer.h"
#include "BinPacker.h"
#include "ResourceManager.h"

namespace de
{
    namespace graphics
    {
        class SpriteSheet
        {
            public:
                SpriteSheet( const std::string &_dir,
                             const std::string &_spriteName,
                             int _width,
                             int _height );

                virtual ~SpriteSheet();

                bool generateSpriteSheet();
            private:
                void refresh();
                void getFiles();
                void getSprites();
                void packTexture();
                void renderToTexture();
                void generateCoordsFile();

                int width, height;
                std::string stringDir, spriteName;
                boost::filesystem::path dir, fileName;

                BinPacker packer;
                lua_State *luaState;


                de::graphics::SpriteBuffer spriteBuffer;
                de::graphics::FrameBufferObject renderTexture;


                std::vector<boost::filesystem::path> foundFiles;
                std::vector<boost::filesystem::path>::iterator iter, end;

                std::vector<de::resources::ImageResource> Images;
                std::vector<de::resources::ImageResource>::iterator iterImage, endImage;

                std::vector<de::classes::Sprite> Sprites;
                std::vector<de::classes::Sprite>::iterator iterSprite, endSprite;


                std::vector<int> rects;
                std::vector<int>::iterator iterInt, endIter;
                std::vector< std::vector<int> > packs;
        };
    }
}
#endif // SPRITESHEET_H
