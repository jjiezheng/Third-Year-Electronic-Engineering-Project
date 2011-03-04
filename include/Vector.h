#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>


namespace de
{
    namespace classes
    {
        //! A 2D vector class, has a x, has a y.
        class Vector
        {
            friend std::ostream& operator<< ( std::ostream& output, const Vector& _vector );
            public:
                //! The Vectors location Horizontally.
                float x;
                //! The Vectors location Horizontally.
                float y;

                float getX() const  { return x; }
                float getY() const  { return y; }

                //! Basic Constructor.
                inline Vector( void ) : x( 0 ), y( 0 ) {}
                //! Normal Constructor.
                inline Vector( float _x, float _y ) : x( _x ), y( _y ) {}


                //! Operator Overloading
                inline Vector &operator /=( const float Scalar )	{ x /= Scalar; y /= Scalar;		return *this; }
                //! Operator Overloading
                inline Vector &operator *=( const float Scalar )	{ x *= Scalar; y *= Scalar;		return *this; }
                //! Operator Overloading
                inline Vector &operator +=( const Vector &Other )   { x += Other.x;  y += Other.y;	return *this; }
                //! Operator Overloading
                inline Vector &operator -=( const Vector &Other )   { x -= Other.x;  y -= Other.y;	return *this; }

                //! Operator Overloading
                inline bool operator !=(  const Vector &rhs )
                {
                    if( x == rhs.x && y == rhs.y )
                        return false;
                    else return true;
                }
                //! Operator Overloading
                inline bool operator ==( const Vector &rhs )
                {
                    if( x == rhs.x && y == rhs.y )
                        return true;
                    else return false;
                }


                //! cross product
                inline float operator ^  ( const Vector &V )  const	{	return ( x * V.y ) - ( y * V.x ); }
                //! dot product
                inline float operator *  ( const Vector &V )  const	{	return ( x * V.x ) + ( y * V.y ); }

                //! Operator Overloading
                inline Vector operator * ( float  s )		 const	{	return Vector( x*s, y*s ); }
                //! Operator Overloading
                inline Vector operator / ( float  s )		 const	{	return Vector( x/s, y/s ); }
                //! Operator Overloading
                inline Vector operator + ( const Vector &V ) const	{	return Vector( x + V.x, y + V.y ); }
                //! Operator Overloading
                inline Vector operator - ( const Vector &V ) const	{	return Vector( x - V.x, y - V.y ); }

                //! Operator Overloading
                friend Vector operator * ( float k, const Vector& V ) {	return Vector( V.x*k, V.y*k ); }

                //! Operator Overloading
                inline Vector operator -( void ) const { return Vector( -x, -y ); }
                //! Get the Vectors length
                inline float length( void ) const { return (float) sqrt( x*x + y*y ); }
                //! Set the Vector to 0.
                inline void clear( void ) { x = y = 0; }

                //! Get the Perpendicular Vector.
                Vector perp() const
                {
                    return Vector( -y, x );
                }

                //! Rotate the Vector in respect to the orgin. (0,0)
                Vector &rotate( const float &angle )
                {
                    float tx = x;
                    x =  x * cos( angle ) - y * sin( angle );
                    y = tx * sin( angle ) + y * cos( angle );
                    return *this;
                }

                //! Rotate the Vector in respect to the orgin (0,0) with cos and sin already worked out.
                Vector &rotate( const float &cosine, const float &sinine )
                {
                    float tx = x;
                    x =  x * cosine  -  y * sinine;
                    y = tx * sinine  +  y * cosine;
                    return *this;
                }

                //! Transform and rotate the vector.
                Vector &transform( const Vector& trans, const float &rot )
                {
                    Vector D = *this;
                    if( rot != 0 )
                        D.rotate( rot );
                    *this = trans + D;
                    return *this;
                }

                //! Normalise the Vector.
                Vector &normalise()
                {
                    Vector D = *this;
                    D = D/D.length();

                    *this = D;
                    return *this;
                }
        };
    }
}




#endif //VECTOR_H
