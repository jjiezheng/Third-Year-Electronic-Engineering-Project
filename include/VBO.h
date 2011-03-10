#ifndef VBO_H
#define VBO_H

#include "Base.h"

namespace de
{
	namespace graphics
	{
		//Move these from here.
		struct AttInfo
		{
			AttInfo(): size(0), offset(0), stride(0) {}
			AttInfo(int _size, int _offset, int _stride, int _normalised): size(_size), offset(_offset), stride(_stride), normalised(_normalised) {}
			int size, offset, stride, normalised;
		};

		class VBO
		{
			public:
				VBO();
				~VBO();
				bool setup();
				void bind();
				void render();
			public:
				Uint32 vertexBuffer, elementBuffer, triangles;
				std::string meshName;
				std::vector<std::string> textures;
				std::map< std::string, AttInfo >  Attribs;
		};
	}
}

#endif // VBO_H