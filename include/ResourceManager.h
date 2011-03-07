#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "Settings.h"
#include "Base.h"
#include "VBO.h"
#include "frameDetails.h"
#include "IResources.h"



namespace de
{
    namespace resources
    {
		class ImageResource;
		class ShaderResource;
		class MusicResource;
		class SoundEffectResource;
		class MeshResource;

        enum ResourceChoices
        {
            None = 0,
            TEXTURES = 1,
			MESHES = 1 << 1,
            SHADERS = 1 << 2,
            MATERIALS = 1 << 3,
            MUSIC = 1 << 4,
            SOUNDEFFECTS = 1 << 5
        };

        //! Handles all Resources ( Textures, Shaders, Music and Sound ).
        /*! This class handles all Resources.
            You can get opaque handles to anything it handles with getTexture(), getShader(), getSoundEffect() and getMusic().
            For example, if you want a texture then you can get an handle to it with getTexture().
        */
        class ResourceManager : public IResources
        {
            public:
                ResourceManager( de::graphics::VideoInfo _videoInfo, int _choice = resources::None );
                virtual ~ResourceManager();


                //! Frees anything passed in thats a ResourceChoices. Uses bitwise to allow muliple options at the same time.
                void free( int _choice );
                //! Loads anything passed in thats a ResourceChoices. Uses bitwise to allow muliple options at the same time.
                void load( int _choice );

                void load( const std::string &_choice );
                void free( const std::string &_choice );

                //! Returns texture coords for the the sprite _name
                /*! The format for _name is <spritesheet>.<Sprite> Say you want the sprite called coolShip on the spritesheet called Ships.
                    The _name you would pass in would be "Ships.coolShip" */
                de::classes::Poly getSprite( const std::string &_name );
                //! Returns texture coords for the the sprites in the vector passes in. see getSprite.
                const std::vector<de::classes::Poly>& getSprites( const std::vector<std::string> &_names );

                de::classes::Frect getSpriteCoords( const std::string &_name, bool tex = true );
                std::map<std::string, de::classes::Frect>& getSpritesCoords( const std::vector<std::string> &_names, bool tex = true );
                std::map<std::string, de::classes::Frect>& getSpritesCoords( const std::string &_texture, bool tex = true );
                std::vector<std::string>& getSpritesName( const std::string &_texture );


                //! Loads the Texture "_texture" and returns an opaque handle.
                const Uint32& getTexture( const std::string &_texture );
                //! Loads the Shader "_shaderName" and returns a Shader Object.
                const de::graphics::Shader& getShader( const std::string &_shaderName );
                //! Loads the SoundEffect "_soundEffect" and returns an opaque handle.
                const int& getSoundEffect( const std::string &_soundEffect );
                //! Loads the Music "_music" and returns an opaque handle.
                const int& getMusic( const std::string &_music );

				const graphics::VBO& getMesh( const std::string &_mesh );

                //! Gets the Texture Size of "_texture" with "_width" and "_height". (Caches the result)
                bool getTextureSize( const std::string &_texture, float &_width, float &_height );

            private:
                void loadShaders( std::map<std::string, ShaderResource> &_shaders );
                void loadTextures( std::map<std::string, ImageResource> &_textures );
                void loadMusic( std::map<std::string, MusicResource> &_music );
                void loadSoundEffects( std::map<std::string, SoundEffectResource> &_soundEffects );
                void loadMeshes( std::map<std::string, MeshResource> &_meshes );

                void shaderFreeAll( std::map<std::string, ShaderResource> &_shaders );
                void textureFreeAll( std::map<std::string, ImageResource> &_textures );
                void musicFreeAll( std::map<std::string, MusicResource> &_music );
                void soundEffectFreeAll( std::map<std::string, SoundEffectResource> &_soundEffects );
				void MeshesFreeAll( std::map<std::string, MeshResource> &_meshes );


                void loadTextureResources( const boost::filesystem::path &_path, std::map<std::string, ImageResource> &_textures );
                void loadShaderResources();
                void loadMusicResources();
                void loadSoundEffectResources();
                void loadMeshResources();

            private:
                std::map<std::string, ShaderResource> shaders, shadersExtra;
                std::map<std::string, ShaderResource>::iterator shaderIter;

                std::map<std::string, ImageResource> textures, texturesExtra;
                std::map<std::string, ImageResource>::iterator textureIter;

                std::map<std::string, MusicResource> musics, musicsExtra;
                std::map<std::string, MusicResource>::iterator musicIter;

                std::map<std::string, SoundEffectResource> soundEffects, soundEffectsExtra;
                std::map<std::string, SoundEffectResource>::iterator soundEffectIter;
				
                std::map<std::string, MeshResource> meshes, meshesExtra;
                std::map<std::string, MeshResource>::iterator meshIter;

                lua_State *luaShaderLoader, *luaTextureLoader;

                de::graphics::glslVersion::glslVersions GLSL;
                std::string extraLocation;
        };
    }
}


#endif //RESOURCE_MANAGER_H_

