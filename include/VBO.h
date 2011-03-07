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
			int size, offset, stride;
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
				Uint32 vertexBuffer, elementBuffer;
				std::string meshName;
				std::map< std::string, AttInfo >  Attribs;
		};
	}
}

#endif // VBO_H