#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP

#include "Point.hpp"
#include <vector>
#include <cmath>


class Contour
{
public:
    Contour(int radius, Point center, int numPoints); // circle contour

    Contour(int radius_x, int radius_y, Point center, int numPoints); // ellipse contour

    int Size() const;
    Point* getContourPoints_ptr();

    std::tuple<float, float> secondDiff(int index);
    std::tuple<float, float> fourthDiff(int index);

    float TensionEnergyAtPoint(int idx, Point newPoint) const;
    float CurveEnergyAtPoint(int idx, Point newPoint) const;
    float secondDiff(const Contour contour, int index);
    
    Point &operator[](int idx);
    const Point &operator[](int idx) const;
    
    private:
    
    static constexpr double PI = 3.14159265358979323846;
    
    Point m_center;
    int m_numPoints;
    std::vector<Point> m_ContourPoints;
    
    void FillCircleContourPoints(int radius, Point center);
    void FillEllipseContourPoints(int radius_x, int radius_y, Point center);
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP

