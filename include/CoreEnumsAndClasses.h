#ifndef BASIC_ENUMS_AND_CLASSES_H
#define BASIC_ENUMS_AND_CLASSES_H

#include "Material.h"
#include "Functions.h"
#include "Vector.h"
#include "Log.h"

#include "Base.h"


namespace de
{/*
    namespace graphics
    {
        class Shader;
    }*/

    namespace filesystem
    {
        namespace root
        {
            enum roots
            {
                RESOURCES,
                TEXTURES,
                SHADERS,
                MESHES,
                FONTS,
                MUSIC,
                SOUNDEFFECTS,
                SETTINGS,
                SHIPS,
                BULLETS,
                PARTICLES,
                LUA
            };
        }

        class Root
        {
            public:
                Root();
                ~Root();

                const std::string get( root::roots _choice );

            private:
                std::string resources, textures, shaders, meshes, fonts, music, soundeffects, settings, ships, bullets, particles, lua;
                lua_State *luaState;
        };

        extern Root* Roots;

    }

    namespace classes
    {
        //! A box struct.
        class Frect
        {
            friend std::ostream& operator<< ( std::ostream& output, const Frect& _rect );
            public:
                //! Basic Constructor
                Frect() : x(0), y(0), w(0), h(0) {}
                Frect( const float &_x, const float &_y, const float &_w, const float &_h ) :
                    x(_x), y(_y), w(_w), h(_h) {}

                //! Position Horizontally.
                float x;
                //! Position Vertically.
                float y;
                //! Width.
                float w;
                //! Height.
                float h;
        };

        //! A RGBA Colour struct.
        class Colour
        {
            friend std::ostream& operator<< ( std::ostream& output, const Colour& _colour );

            public:
                //! Basic Constructor
                Colour() : r(1), g(1), b(1), a(1) {}
                //! Normally used Constructor
                Colour( const float &_r, const float &_g, const float &_b, const float &_a )
                    : r(_r), g(_g), b(_b), a(_a) {}

                //! Operator overloading for checking to see if two colours are equal.
                bool operator == ( const Colour &_other )
                {
                    if( r == _other.r &&
                        g == _other.g &&
                        b == _other.b &&
                        a == _other.a )
                    {
                        return true;
                    }
                    else return false;
                };

                //! Operator overloading for checking to see if two colours are not equal.
                bool operator != ( const Colour &_other )
                {
                    return( !( *this == _other ) );
                };
            public:
                //! Red
                float r;
                //! Green
                float g;
                //! Blue
                float b;
                //! Alpha
                float a;
        };




        class BasicSettings
        {
            public:
                static float globalScreenRatio;
                static Vector  mouseOffset;
                static std::string rootPrefix;

                static bool borderHorizontal;
        };






        //! A Polygon class. Used mostly for coords and hitboxes.
        /*! Can only work with four sided Polygon boxes but could be made more general if needed.
        */

        class Poly
        {
            friend std::ostream& operator<< ( std::ostream& output, const Poly& _poly );
            public:
                enum { MAX_VERTICES = 4 };
                //! A vector of the Polys points.
                std::vector<Vector>	Vertices;

                //! The position of the Polys centre.
                Vector centre;
                //! The direction the Poly was last pointing at.
                Vector last;

            public:
                //! Basic Constructor.
                Poly();
                //! Constructor used for texture coords.
                Poly( const float &_width, const float &_height, bool _centre = false );
                //! Normal Constructor.
                Poly( const Frect &_A, int angle, bool _centre = false );

                //! Normally used after the Basic Constructor.
                void Setup( const Frect& _A, int angle );
                //! same as the texture coords Constructor.
                void Setup( const float &_width, const float &_height, bool _centre = false );

                //! Transform the Poly by "_position" and rotate by "_rotation".
                void transform( const Vector &_position, const float &_rotation );
                //! Transform the Poly by "_position".
                void transform( const Vector& _position );
                //! Transform the Poly by "_position" and rotate depending on the Vector "last". Used for pointing the Poly in the direction of the transform.
                void transformRotate( const Vector& _position );

                //! Resize the Poly by "_newSize". 1 means no change in size.
                void resize( const float & );

                void setCentre( const Vector& _position );


                //! Rotate around the Poly centre by "_rotation"
                void rotateCentre( const float &_rotation );
                void rotateCentre( const float &_cos, const float &_sin );

                void rotateFrom( const float &_rotation, const Vector &_position );
                void rotateFrom( const float &_cos, const float &_sin, const Vector &_position );

                //! Acts very like void Setup( float, float ); Marked for Deletion.
                void changeRect( const int &_width, const int &_height );

                //! Flide the poly vertically or horizonally depending on if a bool "true" is passed in.
                void flip( bool _horizonally = false );

                const float width();
                const float height();

                //! Get the Miinimum and Maximum points of the Poly
                void getMinMax( Vector &_min, Vector &_max );
                //! Checks for overlap with _poly. Uses Seperating Axis Theorem.
                bool collide( const Poly &_poly ) const;
                //! Checks to see if the point "_point" is inside the Poly. Returns true if yes. Uses Barrycentric coordinates.
                bool containsPoint( const Vector &_point );

                void render( const de::classes::Colour &_colour, const int &_type );

            private:
                int Count;
                float radiusSqrd;

            private:
                void calculateInterval(const Vector &_axis, float &_min, float &_max) const;
                bool intervalsSeparated( float &_aMin, float &_aMax, float &_bMin, float &_bMax ) const;
                bool separatedByAxis( const Vector& _axis, const Poly& _poly) const;
                bool isPointInTriangle( const Vector& _A, const Vector& _B, const Vector& _C, const Vector& _P ) const;
        };

        //! A Sprite struct.
        /*! The main graphics struct an in game object is going to have. Can be rendered on screen with Graphics::addSprite (const de::classes::Sprite &_Sprite, const int &_type)
        */
        class Sprite
        {
            friend std::ostream& operator<< ( std::ostream& output, const Sprite& _sprite );
            public:
                //! Basic Constructor
                Sprite() : Texture(0), Texture2(0), coords(), texCoords( 1.0, 1.0 ),texCoords2( 1.0, 1.0 ), colour(), priority(0) {}
                Sprite( const std::string &_texture, const std::string &_shader );
                ~Sprite() {}

                void setTexture( const std::string &_texture, const std::string &_shader = "Basic" );
                void setTexture( const Uint32 &_texture, const std::string &_shader = "Basic" );
                void setShader( const std::string &_shader = "Basic" );
                void setTexCoords( const std::string &_sprite );
                void reload();

                inline const Uint32& getTexture() const
                {
                    return Texture;
                }

                const de::graphics::Shader& getShader() const;


                inline void setPriority( const int &_priority )
                {
                    priority = _priority;
                }

                inline void setColour( const Colour &_colour )
                {
                    colour = _colour;
                }

                inline void setCoords( const Poly &_coords )
                {
                    coords = _coords;
                }

                inline void setTexCoords( const Poly &_texCoords )
                {
                    texCoords = _texCoords;
                }

                inline void transform( const Vector &_vector, const float &_rotation, bool _fromCentre )
                {
                    if( _fromCentre )
                    {
                        coords.transform( _vector - coords.centre, _rotation );
                    }
                    else
                    {
                        coords.transform( _vector, _rotation );
                    }
                }

                void render( const int &_type );



            private:
                //! Handle to the Sprites Texture.
                Uint32 Texture;
                //! Shader
                graphics::Shader shader;

                //! String values for the texture and shader
                std::string textureName, shaderName;

            public:
                //! Handle to the Sprites second Texture.
                Uint32 Texture2;
                //! Coords of the Sprites location onscreen.
                Poly coords;
                //! Coords of the Sprites Texture.
                Poly texCoords;
                //! Coords of the Sprites second Texture.
                Poly texCoords2;
                //! Handle to the Sprites Shader.
                Colour colour;
                //! The sprites priority. The higher a sprites priority, the later it will be rendered on screen. ( It will be on top of everything else. )
                int priority;
        };
    }


    namespace theme
    {
        static classes::Colour
            DefaultTextColour( 0.89019f,0.847058f, 0.71372f, 1 ),
            DefaultBoxColour( 0.40392f, 0.411764f, 0.372549f, 1 ),
            DefaultSelectedBoxColour( 0.168627f,0.145098f,0.1647058f, 1 );
    }
}


#endif //BASIC_ENUMS_AND_CLASSES_H

