#ifndef GRAPHER_H
#define GRAPHER_H

#include "CoreEngine.h"
#include "Coords.h"

class Grapher
{
    public:
        Grapher( const std::string &_name );
        virtual ~Grapher();

        //void add( unsigned int _time, de::enums::de_GraphItems _itemType );
    private:
        std::string name;
};

#endif // GRAPHER_H
