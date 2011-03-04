#ifndef FONT_H_
#define FONT_H_

#include "Monitor.h"
#include "Settings.h"
#include "CoreEnumsAndClasses.h"

#include <iostream>
#include <vector>
#include <ostream>

namespace de
{
    namespace graphics
    {
        class Shader;
    }

    namespace enums
    {
        enum Align
        {
            ALIGN_LEFT = 1,
            ALIGN_RIGHT = 2,
            ALIGN_CENTRE = 3,
            ALIGN_NO = 0
        };
    }



    namespace internal
    {
        //! A single character. Internal struct used by Font
        struct CharDescriptor
        {
            classes::Frect rect;
            unsigned short XOffset, YOffset;
            unsigned short XAdvance;
            unsigned short Page;

            CharDescriptor() : XOffset( 0 ), YOffset( 0 ),
                XAdvance( 0 ), Page( 0 )
            { }
        };

        //! A single font. Internal struct used by Font
        struct BitmapFont
        {
            std::string name;
            classes::Sprite sheet;

            float texWidth, texHeight;
            unsigned short LineHeight;
            unsigned short Base;
            unsigned short Width, Height;
            unsigned short Pages;
            CharDescriptor Chars[256];
        };
    }

    namespace core
    {
        //! Font class for loading and using bitmap fonts.
        class Font
        {
        public:

            //! Make the font "_fontName" the active font.
            static bool SelectFont( const std::string &_fontName );
            //! Free all fonts.
            static void freeAll();
            //! Reload fonts. Used when OpenGl context is changed.
            static void reLoadTextures();

            //! Write the message "_message" at the location "_pos"
            static const std::vector<de::classes::Sprite>& writeMessage( const std::string &_message, const de::classes::Vector &_pos, int _priority = 11 );

        private:
            static std::vector<de::internal::BitmapFont> Fonts;
            static std::vector<de::internal::BitmapFont>::iterator selectedFont;

            static bool addFont( const std::string &_name );
            static bool parseFont( std::istream& Stream, de::internal::BitmapFont& CharsetDesc );
        };

    }

    namespace classes
    {
        class SpriteString
        {
            friend std::ostream& operator<< ( std::ostream& output, const SpriteString& _string );
            friend std::istream& operator>> ( std::istream& output, SpriteString& _string );

            public:
                SpriteString();
                SpriteString( const std::string &_text,
                              const std::string &_font,
                              const Vector &_position,
                              const int &_priority = 11,
                              enums::Align _align = enums::ALIGN_NO );

                ~SpriteString();

                void set( const std::string &_text,
                          const std::string &_font,
                          const Vector &_position,
                          const int &_priority = 11,
                          enums::Align _align = enums::ALIGN_NO );

                void setText( const std::string &_text );
                void setAlpha( const float &_Alpha );
                void setColour( const Colour &_colour );
                void setShader( const std::string &_shader );
                void Align( enums::Align _align = enums::ALIGN_NO );
                void reload();
                std::vector<Sprite> & getSprites();
                void render( const int &_type );


                inline const std::string& operator = ( const std::string &Other )
                {
                    if( text != Other )
                    {
                        reloadSprites = true;
                        text = Other;
                    }

                    return text;
                }

                //! Operator overloading
                inline Vector operator = ( const Vector &_position )
                {
                    reloadSprites = true;
                    position = _position;
                    return position;
                }

                //! Operator overloading
                inline std::string & operator + ( const std::string &Other )
                {
                    reloadSprites = true;
                    text += Other;
                    return text;
                }

                //! Operator overloading
                inline const char* c_str()  { return text.c_str(); }

            private:
                bool reloadSprites, isSet;
                enums::Align align;
                int priority;
                float Alpha;
                Vector position;
                Colour colour;
                std::string text, font;
                graphics::Shader shader;
                std::string shaderType;
                std::vector<Sprite> Sprites;
        };
    }
}
#endif //FONT_H_
