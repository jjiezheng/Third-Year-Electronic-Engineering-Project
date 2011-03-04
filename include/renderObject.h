#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

namespace de
{
    namespace graphics
    {
        struct vertex
        {
            float r,g,b,a;

            vertex() : r(0), g(0), b(0), a(0) {}
            vertex( float _r, float _g, float _b, float _a )
                : r(_r), g(_g), b(_b), a(_a) {}
        };
        vertex normalise( const vertex &_aVertex );

        class renderObject
        {
            public:
                /** Default constructor */
                renderObject();
                /** Default destructor */
                virtual ~renderObject();

                virtual void reload() = 0;
                virtual void render() = 0;
                virtual void localRender() = 0;
            protected:
            private:
        };
    }
}

#endif // RENDEROBJECT_H
