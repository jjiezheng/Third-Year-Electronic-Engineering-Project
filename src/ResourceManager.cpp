#include "ResourceManager.h"
#include "GameServices.h"

#include "CoreEnumsAndClasses.h"
#include <luabind/luabind.hpp>
#include <corona.h>


using namespace de::misc;
using namespace de::enums;
using namespace de::graphics;
using namespace de::filesystem;
using namespace de::classes;

namespace fs = boost::filesystem;

namespace de
{
    namespace resources
    {

        ResourceManager::ResourceManager( VideoInfo _videoInfo, int _choice )
        {
            GLSL = _videoInfo.GLSL;

            loadTextureResources( fs::path( Roots->get( root::TEXTURES ) ), textures );
            loadTextureResources( fs::path( Roots->get( root::FONTS ) ), textures );

            loadShaderResources();
            loadMusicResources();
            loadSoundEffectResources();

            load( _choice );
        }

        ResourceManager::~ResourceManager()
        {
            free( TEXTURES | SHADERS | MUSIC | SOUNDEFFECTS );
        }




        void ResourceManager::loadTextureResources( const boost::filesystem::path &_path, std::map<std::string, ImageResource> &_textures )
        {
            luaTextureLoader = lua_open();
            luaL_openlibs(luaTextureLoader);
            luabind::open(luaTextureLoader);

            std::vector<boost::filesystem::path> foundFiles;
            std::vector<boost::filesystem::path>::iterator iter, end;

            getFilesFrom( _path, foundFiles, filetypes::IMAGES );

            for( iter = foundFiles.begin(); iter < foundFiles.end(); ++iter )
            {
                std::string textureName( stripFileEnding( iter->filename() ) );
                _textures[textureName] = ImageResource( textureName, (*iter) );
            }
        }

        void ResourceManager::loadShaderResources()
        {
            if( glslVersion::none < GLSL )
            {
                luaShaderLoader = lua_open();
                luaL_openlibs(luaShaderLoader);

                std::string shaderLoader = Roots->get( root::SHADERS ) + "ShaderLoader.lua";
                luaL_dofile( luaShaderLoader, shaderLoader.c_str() );
            }
        }

        void ResourceManager::loadMusicResources()
        {
            std::vector<boost::filesystem::path> foundFiles;
            std::vector<boost::filesystem::path>::iterator iter, end;

            fs::path musicFilePath( Roots->get( root::MUSIC ) );
            getFilesFrom( musicFilePath, foundFiles, filetypes::MUSIC );

            for( iter = foundFiles.begin(); iter < foundFiles.end(); ++iter )
            {
                std::string musicName( stripFileEnding( iter->filename() ) );
                musics[musicName] = MusicResource( musicName, (*iter) );
            }
        }

        void ResourceManager::loadSoundEffectResources()
        {
            std::vector<boost::filesystem::path> foundFiles;
            std::vector<boost::filesystem::path>::iterator iter, end;

            fs::path soundEffectsFilePath( Roots->get( root::SOUNDEFFECTS ) );
            getFilesFrom( soundEffectsFilePath, foundFiles, filetypes::SOUNDEFFECTS );

            for( iter = foundFiles.begin(); iter < foundFiles.end(); ++iter )
            {
                std::string soundEffectName( stripFileEnding( iter->filename() ) );
                soundEffects[soundEffectName] = SoundEffectResource( soundEffectName, (*iter) );
            }
        }



        void ResourceManager::loadShaders( std::map<std::string, ShaderResource> &_shaders )
        {
            if( glslVersion::none < GLSL )
            {
                for ( shaderIter = _shaders.begin(); shaderIter != _shaders.end(); ++shaderIter )
                {
                    shaderIter->second.loadUsing( luaShaderLoader );
                }
            }
        }

        void ResourceManager::loadMusic( std::map<std::string, MusicResource> &_music )
        {
            for ( musicIter = _music.begin(); musicIter != _music.end(); ++musicIter )
            {
                musicIter->second.load();
            }
        }

        void ResourceManager::loadSoundEffects( std::map<std::string, SoundEffectResource> &_soundEffects )
        {
            for ( soundEffectIter = _soundEffects.begin(); soundEffectIter != _soundEffects.end(); ++soundEffectIter )
            {
                soundEffectIter->second.load();
            }
        }

        void ResourceManager::loadTextures( std::map<std::string, ImageResource> &_textures )
        {
            for( textureIter = _textures.begin(); textureIter != _textures.end(); ++textureIter )
            {
                textureIter->second.load( luaTextureLoader );
            }
        }


        void ResourceManager::textureFreeAll( std::map<std::string, ImageResource> &_textures )
        {
            for( textureIter = _textures.begin(); textureIter != _textures.end(); ++textureIter )
            {
                textureIter->second.unload();
            }
        }

        void ResourceManager::shaderFreeAll( std::map<std::string, ShaderResource> &_shaders )
        {
            if( glslVersion::none < GLSL )
            {
                for( shaderIter = _shaders.begin(); shaderIter != _shaders.end(); ++shaderIter )
                {
                    shaderIter->second.unload();
                }
            }
        }

        inline void ResourceManager::musicFreeAll(  std::map<std::string, MusicResource> &_music )
        {
            for( musicIter = _music.begin(); musicIter != _music.end(); ++musicIter )
            {
                musicIter->second.unload();
            }
        }

        inline void ResourceManager::soundEffectFreeAll( std::map<std::string, SoundEffectResource> &_soundEffects )
        {
            for( soundEffectIter = _soundEffects.begin(); soundEffectIter != _soundEffects.end(); ++soundEffectIter )
            {
                soundEffectIter->second.unload();
            }
        }

        void ResourceManager::free( int _choice )
        {
            if ( (_choice & TEXTURES) == TEXTURES )
            {
                textureFreeAll( textures );
                textureFreeAll( texturesExtra );
            }

            if ( (_choice & SHADERS) == SHADERS )
            {
                shaderFreeAll( shaders );
            }

            if ( (_choice & MUSIC) == MUSIC )
            {
                musicFreeAll( musics );
            }

            if ( (_choice & SOUNDEFFECTS) == SOUNDEFFECTS )
            {
                soundEffectFreeAll( soundEffects );
            }
        }

        void ResourceManager::load( int _choice )
        {
            if ( (_choice & TEXTURES) == TEXTURES )
            {
                loadTextures( textures );
                loadTextures( texturesExtra );
            }

            if ( (_choice & SHADERS) == SHADERS )
            {
                loadShaders( shaders );
            }

            if ( (_choice & MUSIC) == MUSIC )
            {
                loadMusic( musics );
            }

            if ( (_choice & SOUNDEFFECTS) == SOUNDEFFECTS )
            {
                loadSoundEffects( soundEffects );
            }
        }

        void ResourceManager::free( const std::string &_choice )
        {
        }

        void ResourceManager::load( const std::string &_choice )
        {
            //de::io::log << "Loading extra resources with the command " << _choice << "\n";

            if( extraLocation != _choice )
            {
                //de::io::log << "Loading New at location " << Roots->get( root::RESOURCES ) + _choice + "/Images/\n";
                if( !extraLocation.empty() )
                {
                    textureFreeAll( texturesExtra );
                    texturesExtra.clear();
                }
                extraLocation = _choice;

                loadTextureResources( fs::path(Roots->get( root::RESOURCES ) + extraLocation + "/Images/"), texturesExtra );
                loadTextures( texturesExtra );
            }
        }



        de::classes::Poly ResourceManager::getSprite( const std::string &_name )
        {
            return de::classes::Poly( getSpriteCoords( _name, true ), 0, true );
        }

        const std::vector<de::classes::Poly>& ResourceManager::getSprites( const std::vector<std::string> &_names )
        {
            static std::vector<de::classes::Poly> frames;
            frames.clear();

            for( std::vector<std::string>::const_iterator iter = _names.begin(); iter < _names.end(); ++iter )
                frames.push_back( getSprite( (*iter) ) );

            return frames;
        }

        de::classes::Frect ResourceManager::getSpriteCoords( const std::string &_name, bool tex )
        {
            std::vector<std::string> strs;
            std::vector<std::string>::iterator iter, end;

            boost::split(strs, _name, boost::is_any_of("."));
            iter = strs.begin();
            end = strs.end();

            std::string spriteName, name = (*iter);
            for( iter = strs.begin()+1; iter < end; ++iter )
            {
                spriteName = (*iter);
            }

            textureIter = textures.find( name );
            if( textureIter == textures.end())
            {
                textureIter = texturesExtra.find( name );
                if( textureIter == texturesExtra.end())
                {
                    de::io::error << "Failure to get Sprite with the name " << _name << "\n";
                    return Frect(0,0,0,0);
                }
            }
            return textureIter->second.getSpriteCoords( spriteName, tex );
        }

        std::map<std::string, de::classes::Frect>& ResourceManager::getSpritesCoords( const std::vector<std::string> &_names, bool tex )
        {
            static std::map<std::string, de::classes::Frect> frames;
            frames.clear();

            for( std::vector<std::string>::const_iterator iter = _names.begin(); iter < _names.end(); ++iter )
                frames[(*iter)] = getSpriteCoords( (*iter), tex );

            return frames;
        }

        std::map<std::string, de::classes::Frect>& ResourceManager::getSpritesCoords( const std::string &_texture, bool tex )
        {
            static std::map<std::string, de::classes::Frect> frames;

            textureIter = textures.find( _texture );
            if( textureIter == textures.end())
            {
                textureIter = texturesExtra.find( _texture );
                if( textureIter == texturesExtra.end())
                {
                    return frames;
                }
            }
            frames = textureIter->second.getAllSprites( tex );
            return frames;
        }

        std::vector<std::string>& ResourceManager::getSpritesName( const std::string &_texture )
        {
            static std::vector<std::string> sprites;

            textureIter = textures.find( _texture );
            if( textureIter == textures.end())
            {
                textureIter = texturesExtra.find( _texture );
                if( textureIter == texturesExtra.end())
                {
                    return sprites;
                }
            }
            sprites = textureIter->second.getSpriteNames();
            return sprites;
        }




        const Uint32& ResourceManager::getTexture( const std::string& _texture )
        {
            textureIter = textures.find( _texture );
            if( textureIter == textures.end())
            {
                textureIter = texturesExtra.find( _texture );
                if( textureIter == texturesExtra.end())
                {
                    de::io::error << "Failure to get Texture with the name " << _texture << "\n";
                    static Uint32 blank(0);
                    return blank;
                }
            }
            return textureIter->second.getTexture( luaTextureLoader );
        }

        const de::graphics::Shader& ResourceManager::getShader( const std::string &_shaderName )
        {
            if( glslVersion::none < GLSL )
            {
                shaderIter = shaders.find(_shaderName);
                if( shaderIter == shaders.end() )
                {
                    shaders[_shaderName] = ShaderResource( _shaderName );
                    shaderIter = shaders.find(_shaderName);
                }
                return shaderIter->second.getShaderObject( luaShaderLoader );
            }
            static Shader empty;

            de::io::error << "ResourceManager: Couldn't find shader - " << _shaderName << "\n";
            return empty;
        }

        const int& ResourceManager::getMusic( const std::string& _music )
        {
            musicIter = musics.find( _music );

            if( musicIter == musics.end())
            {
                de::io::error << "Failure to get Music with the name " << _music << "\n";
                static int blank(0);
                return blank;
            }
            return musicIter->second.getMusic();
        }

        const int& ResourceManager::getSoundEffect( const std::string& _soundEffect )
        {
            soundEffectIter = soundEffects.find( _soundEffect );

            if( soundEffectIter == soundEffects.end())
            {
                de::io::error << "Failure to get Music with the name " << _soundEffect << "\n";
                static int blank(0);
                return blank;
            }
            return soundEffectIter->second.getSoundEffect();
        }

        bool ResourceManager::getTextureSize( const std::string& _texture, float &_width, float &_height )
        {
            textures.find( _texture );

            if( textureIter == textures.end())
            {
                return false;
            }

            _width = (float)textureIter->second.width;
            _height = (float)textureIter->second.height;

            return true;
        }




        ShaderResource::ShaderResource() {}

        ShaderResource::ShaderResource( const std::string &_name )
        {
            name = _name;
        }

        ShaderResource::~ShaderResource()
        {
            unload();
        }

        const de::graphics::Shader& ShaderResource::getShaderObject( lua_State* L )
        {
            if( shader.get( graphics::Shader::PROGRAM ) == 0 )
            {
                loadUsing( L );
            }
            return shader;
        }

        bool ShaderResource::loadUsing( lua_State* L )
        {
            Engine::Graphics().loadShader( L, shader, name );
            return true;
        };

        void ShaderResource::unload()
        {
            Engine::Graphics().unloadShader( shader );
        }


        MusicResource::MusicResource( const std::string &_name, const fs::path &_path ) : Music(0)
        {
            name = _name;
            path = _path;
        }

        MusicResource::~MusicResource()
        {
            unload();
        }

        const int& MusicResource::getMusic()
        {
            if( Music == 0 )
            {
                load();
            }
            return Music;
        }

        void MusicResource::load()
        {
            if( Music == 0 )
            {
                Music = Engine::Audio().loadMusic( path.file_string() );
            }
        }

        void MusicResource::unload()
        {
            Engine::Audio().deleteMusic( Music );
            Music = 0;
        }



        SoundEffectResource::SoundEffectResource( const std::string &_name, const fs::path &_path ) : SoundEffect(0)
        {
            name = _name;
            path = _path;
        }

        SoundEffectResource::~SoundEffectResource()
        {
            unload();
        }

        const int& SoundEffectResource::getSoundEffect()
        {
            if( SoundEffect == 0 )
            {
                load();
            }
            return SoundEffect;
        }

        void SoundEffectResource::load()
        {
            if( SoundEffect == 0 )
            {
                SoundEffect = Engine::Audio().loadSoundEffect( path.file_string() );
            }
        }

        void SoundEffectResource::unload()
        {
            Engine::Audio().deleteSoundEffect( SoundEffect );
            SoundEffect = 0;
        }
    }
}
