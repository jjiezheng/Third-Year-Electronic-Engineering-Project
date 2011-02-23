#ifndef FONT_H
#define FONT_H

#include "IFont.h"
#include "Base.h"
#include <map>
#include "renderObject.h"

namespace de
{
	namespace gui
	{
		namespace internal
		{
			struct CharDescriptor
			{
				de::graphics::vertex verts[4];
				de::graphics::vertex texVerts[4];
				glm::vec2 rect[2];
				glm::vec2 Offset;
				unsigned short XAdvance;
				unsigned short Page;

				CharDescriptor() : XAdvance( 0 ), Page( 0 )
				{ }
				void generate( const float &_width, const float &_height );
			};

			//! A single font. Internal struct used by Font
			struct Font
			{
				Font() {}
				Font( const std::string &_name );

				std::string name;
				Uint32 Texture;

				float texWidth, texHeight;
				unsigned short LineHeight;
				unsigned short Base;
				unsigned short Width, Height;
				unsigned short Pages;
				CharDescriptor Chars[256];
			};
		}


		class FontManager : public IFont
		{
			public:
				FontManager();
				virtual ~FontManager();

				bool load( const std::string &_name );
				bool free( const std::string &_name );
				bool freeAll();
				bool reload();

				bool write( const std::string &_message,
					const std::string &_font,
					std::vector<de::graphics::vertex> &_verts, 
					std::vector<de::graphics::vertex> &_TexVerts, 
					std::vector<int> &_elems );
			private:
				void parseFont( std::istream& Stream, internal::Font& CharsetDesc );
				void genText( const std::string &_message,
					internal::Font _font,
					std::vector<de::graphics::vertex> &_verts, 
					std::vector<de::graphics::vertex> &_texVerts, 
					std::vector<int> &_elems );
				
			private:
				std::map< std::string, internal::Font > fonts;
		};
	}
}

#endif // FONT_H