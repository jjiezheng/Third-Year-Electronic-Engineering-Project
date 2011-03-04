#include "CoreEnumsAndClasses.h"
#include "GameServices.h"
#include "Material.h"
#include <luabind/luabind.hpp>

namespace de
{
    namespace filesystem
    {
        Root* Roots = NULL;

            Root::Root()
        {
            luaState = lua_open();
            luabind::open(luaState);
            #ifdef __APPLE__
                luaL_dofile(luaState, "roots.lua");
                std::string table("Mac");
            #else
                luaL_dofile(luaState, "roots.lua");
                std::string table("Windows");
            #endif

            luabind::object global = luabind::globals(luaState);
            luabind::object rootTable = global["Roots"];
            luabind::object localTable = rootTable[table];

            if( luabind::type(localTable) != LUA_TTABLE )
            {
                de::io::error << "In Roots config file." << table << " is not a table!\n";
            }

            resources    = luabind::object_cast<std::string>( localTable["resources"] );
            textures     = luabind::object_cast<std::string>( localTable["textures"] );
            shaders      = luabind::object_cast<std::string>( localTable["shaders"] );
            meshes       = luabind::object_cast<std::string>( localTable["meshes"] );
            fonts        = luabind::object_cast<std::string>( localTable["fonts"] );
            music        = luabind::object_cast<std::string>( localTable["music"] );
            soundeffects = luabind::object_cast<std::string>( localTable["soundeffects"] );
            settings     = luabind::object_cast<std::string>( localTable["settings"] );
            ships        = luabind::object_cast<std::string>( localTable["ships"] );
            bullets      = luabind::object_cast<std::string>( localTable["bullets"] );
            particles    = luabind::object_cast<std::string>( localTable["particles"] );
            lua          = luabind::object_cast<std::string>( localTable["lua"] );

        }


        Root::~Root()
        {
            lua_close(luaState);
        }


        const std::string Root::get( root::roots _choice )
        {
            switch(_choice)
            {
                case( root::RESOURCES )    : return resources;
                case( root::TEXTURES )     : return textures;
                case( root::SHADERS )      : return shaders;
                case( root::MESHES )       : return meshes;
                case( root::FONTS )        : return fonts;
                case( root::MUSIC )        : return music;
                case( root::SOUNDEFFECTS ) : return soundeffects;
                case( root::SETTINGS )     : return settings;
                case( root::SHIPS )        : return ships;
                case( root::BULLETS )      : return bullets;
                case( root::PARTICLES )    : return particles;
                case( root::LUA )          : return lua;
            }
            de::io::error << "No root with that name. :(\n";
            return "";
        }

    }

    namespace classes
    {
        float BasicSettings::globalScreenRatio(1);
        Vector BasicSettings::mouseOffset(0,0);
        bool BasicSettings::borderHorizontal(false);

        #ifdef __APPLE__
            std::string BasicSettings::rootPrefix( "Test.app/Contents/Resources/" );
        #else
            std::string BasicSettings::rootPrefix( "../" );
        #endif





        std::ostream& operator << ( std::ostream& output, const Frect& _rect )
        {
            output << "X: " << _rect.x << "  Y: " << _rect.y << "  W: " << _rect.w << "  H: " << _rect.h;
            return output;  // for multiple << operators.
        }



        std::ostream& operator<< ( std::ostream& output, const Colour &_colour )
        {
            output << "R:" << _colour.r << " G:" << _colour.g << " B:" << _colour.b << " A:" << _colour.a;
            return output;  // for multiple << operators.
        }


        std::ostream& operator<< ( std::ostream& output, const Sprite& _sprite )
        {
            output << "Sprite.textureName: " << _sprite.textureName
                   << "\nSprite.shaderName: "  << _sprite.shaderName
                   << "\nSprite.Texture: "  << _sprite.Texture
                   << "\nSprite.shader: "  << _sprite.shader.get( graphics::Shader::PROGRAM )
                   << "\nSprite.coords: "  << _sprite.coords
                   << "\nSprite.texCoords: "  << _sprite.texCoords
                   << "\nSprite.colour: "  << _sprite.colour
                   << "\nSprite.priority: "  << _sprite.priority;
            return output;
        }



        Sprite::Sprite( const std::string &_texture, const std::string &_shader )
            : Texture(0), Texture2(0), coords(), texCoords( 1.0, 1.0 ),texCoords2( 1.0, 1.0 ), colour(), priority(0)
        {
            setTexture( _texture, _shader );
        }

        const de::graphics::Shader& Sprite::getShader() const
        {
            return shader;
        }

        void Sprite::setTexture( const std::string &_texture, const std::string &_shader )
        {
            textureName = _texture;
            shaderName = _shader;

            Texture = Engine::Resources().getTexture( textureName );
            shader = Engine::Resources().getShader( shaderName );
        }

        void Sprite::setTexture( const Uint32 &_texture, const std::string &_shader )
        {
            textureName = "";
            shaderName = _shader;

            Texture = _texture;
            shader = Engine::Resources().getShader(shaderName );
        }

        void Sprite::setShader( const std::string &_shader )
        {
            shaderName = _shader;
            shader = Engine::Resources().getShader( shaderName );
        }

        void Sprite::setTexCoords( const std::string &_spriteName )
        {
            texCoords = Engine::Resources().getSprite( _spriteName );
        }

        void Sprite::reload()
        {
            Texture = Engine::Resources().getTexture( textureName );
            shader = Engine::Resources().getShader( shaderName );
        }

        void Sprite::render( const int &_type )
        {
            /*Engine::Graphics().add( *this, _type );*/
        }




        std::ostream& operator<< ( std::ostream& output, const Poly& _poly )
        {
            output << "centre: " << _poly.centre << "  Width: " << (_poly.Vertices[3].x - _poly.Vertices[0].x) << "  Height: " << ( _poly.Vertices[3].y - _poly.Vertices[0].y );
            return output;  // for multiple << operators.
        }

        Poly::Poly() :
            last( 0, 1 )
        {
            for( int i = 0; i < MAX_VERTICES; ++i )
                Vertices.push_back( Vector(0,0) );

            Count = 0;
            centre.x = centre.y = 0;
        }


        Poly::Poly( const Frect &_A, int angle, bool _centre )
        {
            for( int i = 0; i < MAX_VERTICES; ++i )
                Vertices.push_back( Vector(0,0) );

            Count = 4;

            Setup( _A, angle );
            if( !_centre )
                transform( Vector( -_A.x, -_A.y ) );
        }


        Poly::Poly( const float &_width, const float &_height, bool _centre ) :
            last( 0, 1 )
        {

            for( int i = 0; i < MAX_VERTICES; ++i )
                Vertices.push_back( Vector(0,0) );

            Count = 4;
            Setup( _width, _height, _centre );
        }

        void Poly::Setup( const Frect &_A, int angle )
        {
            Vector pos( _A.x, _A.y );

            Count = 4;

            Vertices[0].x = -_A.w/2;
            Vertices[0].y = -_A.h/2;

            Vertices[1].x = _A.w/2;
            Vertices[1].y = -_A.h/2;

            Vertices[2].x = -_A.w/2;
            Vertices[2].y = _A.h/2;

            Vertices[3].x = _A.w/2;
            Vertices[3].y = _A.h/2;

            for( int i = 0; i < 4; ++i )
                Vertices[i].transform( pos, angle * misc::piDegrees() );

            centre = Vector(0,0);
            centre += pos;

            //Hacky Hack Hack
            if( _A.w >= _A.h )
                radiusSqrd = (_A.w/2)*(_A.w/2);
            else
                radiusSqrd = (_A.h/2)*(_A.h/2);

        }

        void Poly::Setup( const float &_width, const float &_height, bool _centre )
        {
            Count = 4;

            if( _centre )
            {
                Vertices[0].x = -_width/2;
                Vertices[0].y = -_height/2;

                Vertices[1].x = _width/2;
                Vertices[1].y = -_height/2;

                Vertices[2].x = -_width/2;
                Vertices[2].y = _height/2;

                Vertices[3].x = _width/2;
                Vertices[3].y = _height/2;

                centre = Vector(0,0);
            }
            else
            {
                Vertices[0].x = 0;
                Vertices[0].y = 0;

                Vertices[1].x = _width;
                Vertices[1].y = 0;

                Vertices[2].x = 0;
                Vertices[2].y = _height;

                Vertices[3].x = _width;
                Vertices[3].y = _height;

                centre = Vector(_width/2,_height/2);
            }

            //Hacky Hack Hack
            if( _width >= _height )
                radiusSqrd = (_width/2)*(_width/2);
            else
                radiusSqrd = (_height/2)*(_width/2);
        }

        void Poly::getMinMax( Vector &_min, Vector &_max )
        {
            _min = _max = Vertices[0];

            for( int i = 0; i < Count; ++i )
            {
                if( Vertices[i].x < _min.x )
                    _min.x = Vertices[i].x;

                if( Vertices[i].y < _min.y )
                    _min.y = Vertices[i].y;


                if( Vertices[i].x > _max.x )
                    _max.x = Vertices[i].x;

                if( Vertices[i].y > _max.y )
                    _max.y = Vertices[i].y;
            }
        }



        void Poly::transform( const Vector &_position, const float &_rotation )
        {
            centre += _position;

            for ( int i = 0; i < Count; i ++ )
                Vertices[i].transform( _position, _rotation * misc::piDegrees() );
        }

        void Poly::transform( const Vector &_position )
        {
            centre += _position;

            for ( int i = 0; i < Count; i ++ )
                Vertices[i] += _position;
        }


        void Poly::transformRotate( const Vector &_position )
        {
            if( _position.x != 0 || _position.y != 0 )
            {
                Vector normalised = _position/_position.length();

                float cos = last*normalised;
                float sin = last^normalised;

                for ( int i = 0; i < Count; i ++ )
                {
                    Vertices[i] -= centre;
                    Vertices[i].rotate( cos, sin );
                    Vertices[i] += ( centre + _position );
                }

                centre += _position;
                last = normalised;
            }
        }

        void Poly::resize( const float &_newSize )
        {
            for ( int i = 0; i < Count; ++i )
            {
                Vertices[i] -= centre;
                Vertices[i] *= _newSize;
                Vertices[i] += centre;
            }
        }

        void Poly::setCentre( const Vector& _position )
        {
            for ( int i = 0; i < Count; i ++ )
                Vertices[i] += _position;
        }



        void Poly::rotateCentre( const float &_rotation )
        {
            for ( int i = 0; i < Count; i ++ )
            {
                Vertices[i] -= centre;
                Vertices[i].rotate( _rotation * misc::piDegrees() );
                Vertices[i] += ( centre );
            }
        }

        void Poly::rotateCentre( const float &_cos, const float &_sin )
        {
            for ( int i = 0; i < Count; i ++ )
            {
                Vertices[i] -= centre;
                Vertices[i].rotate( _cos, _sin );
                Vertices[i] += ( centre );
            }
        }

        void Poly::rotateFrom( const float &_rotation, const Vector &_position )
        {
            for ( int i = 0; i < Count; i ++ )
            {
                Vertices[i] -= _position;
                Vertices[i].rotate( _rotation * misc::piDegrees() );
                Vertices[i] += ( _position );
            }

            centre -= _position;
            centre.rotate( _rotation * misc::piDegrees() );
            centre += ( _position );
        }

        void Poly::rotateFrom( const float &_cos, const float &_sin, const Vector &_position )
        {
            for ( int i = 0; i < Count; i ++ )
            {
                Vertices[i] -= _position;
                Vertices[i].rotate( _cos, _sin );
                Vertices[i] += ( _position );
            }

            centre -= _position;
            centre.rotate( _cos, _sin );
            centre += ( _position );
        }


        void Poly::changeRect( const int &_width, const int &_height )
        {
            Vertices[0].x = -_width/2;
            Vertices[0].y = -_height/2;

            Vertices[1].x = _width/2;
            Vertices[1].y = -_height/2;

            Vertices[2].x = -_width/2;
            Vertices[2].y = _height/2;

            Vertices[3].x = _width/2;
            Vertices[3].y = _height/2;


            for ( int i = 0; i < Count; i ++ )
                Vertices[i] += centre;

        }

        void Poly::flip( bool _horizonally )
        {
            Vector buffer(0,0);

            if( _horizonally )
            {
              buffer = Vertices[0];

              Vertices[0] = Vertices[2];
              Vertices[2] = buffer;


              buffer = Vertices[1];

              Vertices[1] = Vertices[3];
              Vertices[3] = buffer;
            }
            else
            {
              buffer = Vertices[0];

              Vertices[0] = Vertices[1];
              Vertices[1] = buffer;


              buffer = Vertices[2];

              Vertices[2] = Vertices[3];
              Vertices[3] = buffer;
            }
        }

        const float Poly::width()
        {
            return ( Vertices[3].x - Vertices[0].x );
        }

        const float Poly::height()
        {
            return ( Vertices[3].y - Vertices[0].y );
        }

        void Poly::render( const de::classes::Colour &_colour, const int &_type )
        {
            /*Engine::Graphics().add( *this, _colour, 20, _type );*/
        }



        bool Poly::containsPoint( const Vector &_point )
        {
            bool check1(false), check2(false), check3(false), check4(false);

            check1 = isPointInTriangle( Vertices[0], Vertices[1], Vertices[2], _point );
            check2 = isPointInTriangle( Vertices[1], Vertices[2], Vertices[3], _point );
            check3 = isPointInTriangle( Vertices[0], Vertices[1], Vertices[3], _point );
            check4 = isPointInTriangle( Vertices[0], Vertices[2], Vertices[3], _point );

            if( check1 || check2 || check3 || check4 )
            {
                return true;
            }
            return false;
        }

        bool Poly::collide( const Poly &_poly ) const
        {
            Vector buffer = centre - _poly.centre;
            if( radiusSqrd + _poly.radiusSqrd < buffer.x*buffer.x + buffer.y*buffer.y )
                return false;


            // test separation axes of current polygon
            for ( int j = Count-1, i = 0; i <= 2; j = i, i++ )
            {
                Vector v0 = Vertices[j];
                Vector v1 = Vertices[i];

                Vector edge = v1 - v0; // edge
                Vector axis = edge.perp(); // sep axis is perpendicular ot the edge

                if ( separatedByAxis( axis, _poly ) )
                    return false;
            }

            // test separation axes of other polygon
            for ( int j = _poly.Count-1, i = 0; i <= 2; j = i, i++ )
            {
                Vector v0 = _poly.Vertices[j];
                Vector v1 = _poly.Vertices[i];

                Vector edge = v1 - v0; // edge
                Vector axis = edge.perp(); // sep axis is perpendicular ot the edge

                if ( separatedByAxis( axis, _poly ) )
                    return false;
            }
            return true;
        }

        void Poly::calculateInterval(const Vector &_axis, float &_min, float &_max) const
        {
            _min = _max = ( Vertices[0] * _axis );

            for (int i = 1; i < Count; ++i)
            {
                float d = (Vertices[i] * _axis);
                if ( d < _min )
                    _min = d;
                else if ( d > _max )
                    _max = d;
            }
        }

        bool Poly::intervalsSeparated( float &_aMin, float &_aMax, float &_bMin, float &_bMax ) const
        {
            return ( _aMin > _bMax) || (_bMin > _aMax );
        }

        bool Poly::separatedByAxis( const Vector& _axis, const Poly& _poly ) const
        {
            float aMin, aMax;
            float bMin, bMax;

            calculateInterval( _axis, aMin, aMax );
            _poly.calculateInterval( _axis, bMin, bMax );

            return intervalsSeparated( aMin, aMax, bMin, bMax );
        }

        bool Poly::isPointInTriangle( const Vector& _A, const Vector& _B, const Vector& _C, const Vector& _P ) const
        {
            // Compute vectors
            Vector v0 = _C - _A;
            Vector v1 = _B - _A;
            Vector v2 = _P - _A;

            // Compute dot products
            float dot00 = v0 * v0;
            float dot01 = v0 * v1;
            float dot02 = v0 * v2;
            float dot11 = v1 * v1;
            float dot12 = v1 * v2;

            // Compute barycentric coordinates
            float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
            float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

            // Check if point is in triangle
            return (u > 0) && (v > 0) && (u + v < 1);
        }

    }
}
