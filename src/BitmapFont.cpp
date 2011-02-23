#include "BitmapFont.h"
#include "GameServices.h"
#include "Material.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace de::classes;
using namespace de::internal;


namespace de
{
    namespace classes
    {
        std::ostream& operator << ( std::ostream& output, const SpriteString& _string )
        {
            output << _string.text;
            return output;  // for multiple << operators.
        }

        std::istream& operator >> ( std::istream& input, SpriteString& _string )
        {
            input >> _string.text;
            return input;  // for multiple << operators.
        }


        SpriteString::SpriteString() :
            reloadSprites(false),
            isSet(false),
            priority(0),
            Alpha(1)
        {
        }

        SpriteString::SpriteString( const std::string &_text, const std::string &_font, const Vector &_position, const int &_priority, enums::Align _align )
            :reloadSprites(false),
            isSet(true),
            priority(0),
            Alpha(1)
        {
            set( _text, _font, _position, _priority, _align );
        }
        SpriteString::~SpriteString()
        {

            Sprites.clear();
        }

        void SpriteString::set( const std::string &_text, const std::string &_font, const Vector &_position, const int &_priority, enums::Align _align  )
        {
            reloadSprites = true;
            isSet = true;

            text = _text;
            font = _font;

            position = _position;
            priority = _priority;

            align = _align;
            shaderType = "Text";
        }


        void SpriteString::setText( const std::string &_text )
        {
            reloadSprites = true;
            text = _text;
        }

        void SpriteString::Align( enums::Align _align )
        {
            if( !Sprites.empty() )
            {
                std::vector<Sprite>::iterator iterSprite, iterStartSprite, iterEndSprite;
                iterStartSprite = Sprites.begin();
                iterEndSprite = Sprites.end() - 1;


                float clampHeight(0);
                for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
                {
                    float tempHeight = iterSprite->coords.Vertices[3].y - iterSprite->coords.Vertices[0].y;
                    if( tempHeight > clampHeight )
                    {
                        clampHeight = tempHeight;
                    }
                }

                Vector aligner(0,0);
                aligner.x = iterEndSprite->coords.Vertices[1].x - iterStartSprite->coords.Vertices[0].x;
                aligner.y = clampHeight;






                if( _align ==enums::ALIGN_LEFT )
                {
                    for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
                    {
                        iterSprite->coords.transform( -aligner );
                    }
                }

                else if( _align ==enums::ALIGN_RIGHT )
                {
                    for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
                    {
                        iterSprite->coords.transform( Vector( 0, -aligner.y ) );
                    }

                }

                else if( _align ==enums::ALIGN_CENTRE )
                {
                    for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
                    {
                        iterSprite->coords.transform( Vector( -aligner.x/2, -aligner.y ) );
                    }

                }
            }

        }

        void SpriteString::setAlpha( const float &_Alpha )
        {
            if( colour.a != _Alpha )
            {
                colour.a = _Alpha;

                std::vector<Sprite>::iterator iterSprite;
                for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
                {
                    iterSprite->colour.a = _Alpha;
                }
            }
        }

        void SpriteString::setColour( const Colour &_colour )
        {
            colour = _colour;

            std::vector<Sprite>::iterator iterSprite;
            for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
            {
                iterSprite->colour = colour;
            }
        }

        void SpriteString::setShader( const std::string &_shader )
        {
            shaderType = _shader;
            reloadSprites = true;
        }

        void SpriteString::reload()
        {
            if( isSet )
            {
                de::core::Font::SelectFont( font );
                Sprites = de::core::Font::writeMessage( text, position, priority );
                Align( align );

                std::vector<Sprite>::iterator iterSprite;
                for( iterSprite = Sprites.begin(); iterSprite < Sprites.end(); ++iterSprite )
                {
                    iterSprite->colour = colour;
                    iterSprite->setShader( shaderType );
                }
            }
        }


        std::vector<Sprite> & SpriteString::getSprites()
        {
            if( reloadSprites )
            {
                reload();
                reloadSprites = false;
            }
            return Sprites;
        }

        void SpriteString::render( const int &_type )
        {
            de::time::PerfTimer timer;
            timer.start();
            if( isSet )
            {
                if( reloadSprites )
                {
                    reload();
                    reloadSprites = false;
                }
                Engine::Graphics().add( Sprites, _type );
            }
        }
    }

    namespace core
    {
        std::vector<BitmapFont> Font::Fonts;
        std::vector<BitmapFont>::iterator Font::selectedFont;


        bool Font::addFont( const std::string &_fontName )
        {
            BitmapFont newFont;
            newFont.name = _fontName;


            newFont.sheet.setTexture( _fontName, "Text" );

            if( !Engine::Resources().getTextureSize( _fontName, newFont.texWidth, newFont.texHeight ) )
                de::io::error << "Failure to get texture width and height.\n";


            newFont.sheet.priority = 11;

            //Get font settings----------------------------------------------------
            #ifdef __APPLE__
                std::string root = "Test.app/Contents/Resources/";
            #else
                std::string root = "";
            #endif

            std::string filename = filesystem::Roots->get( filesystem::root::FONTS ) + _fontName + ".fnt";

            std::ifstream myfile( filename.c_str() );
            if ( !myfile.is_open() )
            {
                de::io::error << "Failed to open fonts/" + _fontName + ".fnt\n";
                return false;
            }
            parseFont( myfile,newFont );
            myfile.close();
            //Get font settings----------------------------------------------------

            Fonts.push_back( newFont );
            return true;
        }

        inline bool Font::parseFont( std::istream& Stream, BitmapFont& CharsetDesc )
        {
            std::string Line;
            std::string Read, Key, Value;
            std::size_t i;
            while( !Stream.eof() )
            {
                std::stringstream LineStream;
                std::getline( Stream, Line );
                LineStream << Line;

                //read the line's type
                LineStream >> Read;
                if( Read == "common" )
                {
                    //this holds common data
                    while( !LineStream.eof() )
                    {
                        std::stringstream Converter;
                        LineStream >> Read;
                        i = Read.find( '=' );
                        Key = Read.substr( 0, i );
                        Value = Read.substr( i + 1 );

                        //assign the correct value
                        Converter << Value;
                        if( Key == "lineHeight" )
                            Converter >> CharsetDesc.LineHeight;
                        else if( Key == "base" )
                            Converter >> CharsetDesc.Base;
                        else if( Key == "scaleW" )
                            Converter >> CharsetDesc.Width;
                        else if( Key == "scaleH" )
                            Converter >> CharsetDesc.Height;
                        else if( Key == "pages" )
                            Converter >> CharsetDesc.Pages;
                    }
                }
                else if( Read == "char" )
                {
                    //this is data for a specific char
                    unsigned short CharID = 0;

                    while( !LineStream.eof() )
                    {
                        std::stringstream Converter;
                        LineStream >> Read;
                        i = Read.find( '=' );
                        Key = Read.substr( 0, i );
                        Value = Read.substr( i + 1 );

                        //assign the correct value
                        Converter << Value;
                        if( Key == "id" )
                            Converter >> CharID;
                        else if( Key == "x" )
                            Converter >> CharsetDesc.Chars[CharID].rect.x;
                        else if( Key == "y" )
                            Converter >> CharsetDesc.Chars[CharID].rect.y;
                        else if( Key == "width" )
                            Converter >> CharsetDesc.Chars[CharID].rect.w;
                        else if( Key == "height" )
                            Converter >> CharsetDesc.Chars[CharID].rect.h;
                        else if( Key == "xoffset" )
                            Converter >> CharsetDesc.Chars[CharID].XOffset;
                        else if( Key == "yoffset" )
                            Converter >> CharsetDesc.Chars[CharID].YOffset;
                        else if( Key == "xadvance" )
                            Converter >> CharsetDesc.Chars[CharID].XAdvance;
                        else if( Key == "page" )
                            Converter >> CharsetDesc.Chars[CharID].Page;
                    }
                }
            }
            return true;
        }

        bool Font::SelectFont( const std::string &_name )
        {

            //If the font is already selected then return
            if( !Fonts.empty()  )
            {
                if( selectedFont->name == _name )
                    return true;
            }

            //If the font is already loaded then select it and return
            std::vector<BitmapFont>::iterator iter;
            for( iter = Fonts.begin(); iter < Fonts.end(); iter++ )
            {
                if( iter->name == _name )
                {
                    selectedFont = iter;
                    return true;
                }
            }

            //If all else fails then load the font then select it and return
            if( addFont( _name ) )
            {
                for( iter = Fonts.begin(); iter < Fonts.end(); iter++ )
                {
                    if( iter->name == _name )
                    {
                        selectedFont = iter;
                        return true;
                    }
                }
            }

            //Font does not exist
            return false;
        }

        void Font::freeAll()
        {
            if( Fonts.empty() )
                return;

            Fonts.clear();
        }

        //I have supported the extended ascii character set. Proper unicode support is planned sometime....
        const std::vector<Sprite>& Font::writeMessage( const std::string &_message, const Vector &_position, int _priority )
        {
            static std::vector<Sprite> vectorBuffer;
            vectorBuffer.clear();

            Vector start = _position, position = _position;

            for( unsigned int i = 0; i < _message.size(); ++i )
            {
                //Hacky Hack Hack. :(
                int character = (int)_message[i];
                if( character < 0 )
                    character = 256 + character;

                //Coords-----------------------------------------------------------
                Frect rect = selectedFont->Chars[character].rect;

                Vector offset;
                offset.x = selectedFont->Chars[character].XOffset + rect.w/2;
                offset.y = selectedFont->Chars[character].YOffset + rect.h/2;

                Poly coords( rect, 0 );
                coords.transform( position + offset );
                selectedFont->sheet.coords = coords;
                //Coords-----------------------------------------------------------
                //Texture coords---------------------------------------------------
                Vector texVector( (float)rect.x/selectedFont->texWidth, (float)rect.y/selectedFont->texHeight );
                selectedFont->sheet.texCoords.Setup( (float)rect.w/selectedFont->texWidth, (float)rect.h/selectedFont->texHeight );
                selectedFont->sheet.texCoords.transform( texVector );
                selectedFont->sheet.priority = _priority;
                //Texture coords---------------------------------------------------


                vectorBuffer.push_back( selectedFont->sheet );
                position.x += selectedFont->Chars[character].XAdvance;

            }
            return vectorBuffer;
        }


        void Font::reLoadTextures()
        {
            selectedFont->sheet.reload();
        }
    }
}
