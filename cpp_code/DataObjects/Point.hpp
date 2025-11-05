#ifndef POINT_HPP
#define POINT_HPP

class Point
{

public:
    int X;
    int Y;
    Point(int x, int y);

    void addX(int deltaX);

    void  addY(int deltaY);

private:
};

#endif