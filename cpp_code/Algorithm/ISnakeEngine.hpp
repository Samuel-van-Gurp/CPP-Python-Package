#ifndef ISNAKEENGINE_HPP
#define ISNAKEENGINE_HPP
#include "DataObjects/Point.hpp"

class ISnakeEngine
{
    public:
        ISnakeEngine() {}

        virtual Point* RunSnake(int iterations) = 0;
        virtual bool EvolveContour() = 0;
};

#endif // ISNAKEENGINE_HPP