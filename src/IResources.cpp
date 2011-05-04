#include "IResources.h"
#include "CoreEnumsAndClasses.h"
#include "Material.h"
#include "VBO.h"


namespace de
{
    namespace resources
    {
        de::classes::Frect NullResources::getSpriteCoords( const std::string &_name, bool tex )
        {
            static de::classes::Frect frect;
            return frect;
        }

        std::map<std::string, de::classes::Frect>& NullResources::getSpritesCoords( const std::vector<std::string> &_names, bool tex )
        {
            static std::map<std::string, de::classes::Frect> sprites;
            return sprites;
        }

        std::map<std::string, de::classes::Frect>& NullResources::getSpritesCoords( const std::string &_texture, bool tex )
        {
            static std::map<std::string, de::classes::Frect> sprites;
            return sprites;
        }

        std::vector<std::string>& NullResources::getSpritesName( const std::string &_texture )
        {
            static std::vector<std::string> sprites;
            return sprites;
        }

        const de::graphics::Shader& NullResources::getShader( const std::string &_shaderName )
        {
            static de::graphics::Shader shader;
            return shader;
        }

		const de::graphics::VBO& NullResources::getMesh( const std::string &_mesh )
		{
			static de::graphics::VBO vbo;
            return vbo;
		}

        const unsigned int& NullResources::getTexture( const std::string &_texture )
        {
            static unsigned int texture(0);
            return texture;
        }

        const int& NullResources::getSoundEffect( const std::string &_soundEffect )
        {
            static int soundEffect(0);
            return soundEffect;
        }

        const int& NullResources::getMusic( const std::string &_music )
        {
            static int music(0);
            return music;
        }
    }
}
