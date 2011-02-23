#ifndef LINE_H_
#define LINE_H_

#include "CoreEnumsAndClasses.h"
#include "Vector.h"
#include <vector>




namespace de
{
    namespace graphics
    {
        class Shader;
    }

    namespace classes
    {
        //! A line class with any number of points
        class Line
        {
            public:
                //! Shader
                graphics::Shader shader;
                //! The Lines priority. Higher is better.
                int priority;

                //! A vector of Points of the Line
                std::vector<Vector> Points;
                //! A vector of Colours for each Point on the Line
                std::vector<Colour> Colours;

                //! Iterator
                std::vector<Vector>::iterator iterPoints;
                //! Iterator
                std::vector<Colour>::iterator iterColours;



            public:
                //! Basic Constructor
                Line();

                //! Operator Overloading
                bool operator < ( const Line& other ) const
                {
                    return ( priority < other.priority );
                }
                //! Add a point to the Line
                void add( const Vector &_point, const Colour &_colour )
                {
                    Points.push_back( _point );
                    Colours.push_back( _colour );
                }
                //! Remove the First point on the line.
                void remove1stPoint()
                {
                    iterPoints = Points.begin();
                    iterColours = Colours.begin();

                    iterPoints = Points.erase( iterPoints );
                    iterColours = Colours.erase( iterColours );
                }
                //! Remove all points on the line.
                void clear()
                {
                    Points.clear();
                    Colours.clear();
                }
        };
    }
}
#endif //LINE_H_


