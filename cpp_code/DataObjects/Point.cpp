#include "Point.hpp"

Point::Point(int x, int y)
    : X(x), Y(y)
{
}


void Point::addX(int deltaX)
{
    X += deltaX;
}

void Point::addY(int deltaY)
{
    Y += deltaY;
}