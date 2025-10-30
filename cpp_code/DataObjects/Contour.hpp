#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP

#include "Point.hpp"
#include <vector>
#include <cmath>

class Contour
{
    public:
    Contour(int radius, Point center, int numPoints);
    int Size() const;

private:
    static constexpr double PI = 3.14159265358979323846;
    
    int m_radius;
    Point m_center;
    int m_numPoints;
    std::vector<Point> m_ContourPoints;

    void FillContourPoints();

    public:
        Point& operator[](std::size_t idx);
        const Point& operator[](std::size_t idx) const;
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP
