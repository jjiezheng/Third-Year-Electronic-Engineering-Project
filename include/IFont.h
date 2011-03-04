#ifndef IFONT_H
#define IFONT_H

#include <string>
#include <vector>

namespace de
{
	namespace graphics
	{
		struct vertex;
	}
	namespace gui
	{
		class IFont
		{
			public:
				IFont() {};
				virtual ~IFont() {};

				virtual bool load( const std::string &_name ) = 0;
				virtual bool free( const std::string &_name ) = 0;
				virtual bool freeAll() = 0;
				virtual bool reload() = 0;

				virtual bool write( const std::string &_message,
									const std::string &_font,
									std::vector<de::graphics::vertex> &_verts, 
									std::vector<de::graphics::vertex> &_Texverts, 
									std::vector<int> &_elems ) = 0;

		};

		class NullFont : public IFont
		{
			public:
				NullFont() {};
				virtual ~NullFont() {};

				bool load( const std::string &_name ) { return false; };
				bool free( const std::string &_name ) { return false; };
				bool freeAll(){ return false; };
				bool reload() { return false; };

				bool write( const std::string &_message,
									const std::string &_font,
									std::vector<de::graphics::vertex> &_verts, 
									std::vector<de::graphics::vertex> &_Texverts, 
									std::vector<int> &_elems ) { return false; };
		};
	}
}

#endif // IFONT_H
