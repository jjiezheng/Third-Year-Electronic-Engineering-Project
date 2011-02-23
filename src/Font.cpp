#include "Font.h"

#include "GameServices.h"
#include "CoreEnumsAndClasses.h"

namespace de
{
	namespace graphics
	{
		vertex& operator += ( vertex& vert, const glm::vec2& _displace )
        {
			vert.r = vert.r + _displace.x;
            vert.g = vert.g + _displace.y;
            return vert;  // for multiple operators.
        }

		vertex operator + ( vertex& vert, const glm::vec2& _displace )
        {
			vertex temp(vert);
			temp.r = vert.r + _displace.x;
            temp.g = vert.g + _displace.y;
            return temp;  // for multiple operators.
        }
	}
	namespace gui
	{
		namespace internal
		{
			Font::Font( const std::string &_name )
			{
				name = _name;
				Texture = Engine::Resources().getTexture(_name);
			}

			void CharDescriptor::generate( const float &_width, const float &_height )
			{
				verts[0] = de::graphics::vertex( -rect[1].x/2.0f, -rect[1].y/2.0f, 0.0f, 1.0f );
				verts[1] = de::graphics::vertex(  rect[1].x/2.0f, -rect[1].y/2.0f, 0.0f, 1.0f );
				verts[2] = de::graphics::vertex(  rect[1].x/2.0f,  rect[1].y/2.0f, 0.0f, 1.0f );
				verts[3] = de::graphics::vertex( -rect[1].x/2.0f,  rect[1].y/2.0f, 0.0f, 1.0f );
				
				texVerts[0] = de::graphics::vertex( (rect[0].x)/_width, (rect[0].y)/_height, 0.0f, 1.0f );
				texVerts[1] = de::graphics::vertex( (rect[0].x+rect[1].x)/_width, (rect[0].y)/_height, 0.0f, 1.0f );
				texVerts[2] = de::graphics::vertex( (rect[0].x+rect[1].x)/_width, (rect[0].y+rect[1].y)/_height, 0.0f, 1.0f );
				texVerts[3] = de::graphics::vertex( (rect[0].x)/_width, (rect[0].y+rect[1].y)/_height, 0.0f, 1.0f );
				
				/*
				texVerts[0] = de::graphics::vertex( 0.0f, 0.0f, 0.0f, 1.0f);
				texVerts[1] = de::graphics::vertex( 1.0f, 0.0f, 0.0f, 1.0f );
				texVerts[2] = de::graphics::vertex( 1.0f, 1.0f, 0.0f, 1.0f );
				texVerts[3] = de::graphics::vertex( 0.0f, 1.0f, 0.0f, 1.0f );
				*/
				
				glm::vec2 centre( rect[1].x/2.0f, rect[1].y/2.0f);
				verts[0] += (Offset + centre);
				verts[1] += (Offset + centre);
				verts[2] += (Offset + centre);
				verts[3] += (Offset + centre);
			}
		}
		FontManager::FontManager()
		{
		}
		FontManager::~FontManager()
		{
		}

		bool FontManager::load( const std::string &_name )
		{
			de::io::log << "Loading font " << _name << ".\n";
			fonts[_name] = internal::Font(_name);
			Engine::Resources().getTextureSize( _name, fonts[_name].texWidth, fonts[_name].texHeight );

			std::string filename = filesystem::Roots->get( filesystem::root::FONTS ) + _name + ".fnt";
			std::ifstream myfile( filename.c_str() );
				if ( !myfile.is_open() )
				{
					de::io::error << "Failed to open fonts/" + _name + ".fnt\n";
					return false;
				}
				parseFont( myfile, fonts[_name] );
			myfile.close();
 
			return true;
		}
		bool FontManager::free( const std::string &_name )
		{
			return false;
		}
		bool FontManager::freeAll()
		{
			return false;
		}
		bool FontManager::reload()
		{
			return false;
		}
		bool FontManager::write( const std::string &_message,
					const std::string &_font,
					std::vector<de::graphics::vertex> &_verts, 
					std::vector<de::graphics::vertex> &_TexVerts, 
					std::vector<int> &_elems )
		{

			std::map<std::string,internal::Font>::iterator iter;

			iter = fonts.find( _font );
            if( iter == fonts.end())
            {
				if( load(_font) )
				{
					iter = fonts.find( _font );
				}
				else return false;
			}

			genText( _message, iter->second, _verts, _TexVerts, _elems );
			return true;
		}

		void FontManager::parseFont( std::istream& Stream, internal::Font& CharsetDesc )
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
                            Converter >> CharsetDesc.Chars[CharID].rect[0].x;
                        else if( Key == "y" )
                            Converter >> CharsetDesc.Chars[CharID].rect[0].y;
                        else if( Key == "width" )
                            Converter >> CharsetDesc.Chars[CharID].rect[1].x;
                        else if( Key == "height" )
                            Converter >> CharsetDesc.Chars[CharID].rect[1].y;
                        else if( Key == "xoffset" )
                            Converter >> CharsetDesc.Chars[CharID].Offset.x;
                        else if( Key == "yoffset" )
                            Converter >> CharsetDesc.Chars[CharID].Offset.y;
                        else if( Key == "xadvance" )
                            Converter >> CharsetDesc.Chars[CharID].XAdvance;
                        else if( Key == "page" )
                            Converter >> CharsetDesc.Chars[CharID].Page;
                    }
					CharsetDesc.Chars[CharID].generate( CharsetDesc.texWidth, CharsetDesc.texHeight );
                }
            }
		}

		void FontManager::genText( const std::string &_message,
			internal::Font _font,
			std::vector<de::graphics::vertex> &_verts, 
			std::vector<de::graphics::vertex> &_texVerts, 
			std::vector<int> &_elems )
		{
			glm::vec2 pos(0.0f);

            for( unsigned int i = 0; i < _message.size(); ++i )
            {
                //Hacky Hack Hack.
                int character = (int)_message[i];
                if( character < 0 )
                    character = 256 + character;


				using namespace de::graphics;
				_verts.push_back( _font.Chars[character].verts[0] + pos );
				_verts.push_back( _font.Chars[character].verts[1] + pos );
				_verts.push_back( _font.Chars[character].verts[2] + pos );
				_verts.push_back( _font.Chars[character].verts[3] + pos );

				_texVerts.push_back( _font.Chars[character].texVerts[0] );
				_texVerts.push_back( _font.Chars[character].texVerts[1] );
				_texVerts.push_back( _font.Chars[character].texVerts[2] );
				_texVerts.push_back( _font.Chars[character].texVerts[3] );

				_elems.push_back(4*i + 0);
				_elems.push_back(4*i + 1);
				_elems.push_back(4*i + 2);
				_elems.push_back(4*i + 2);			
				_elems.push_back(4*i + 3);
				_elems.push_back(4*i + 0);

				pos.x += _font.Chars[character].XAdvance;
            }
		}
	}
}
