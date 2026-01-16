#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP

#include "Point.hpp"
#include <vector>
#include <cmath>


class Contour
{
public:
    Contour(int numPoints); // empty contour

    Contour(int radius, Point center, int numPoints); // circle contour

    Contour(int radius_x, int radius_y, Point center, int numPoints); // ellipse contour

    int Size() const;
    std::vector<Point> getContourPoints();

    float TensionEnergyAtPoint(int idx, Point newPoint) const;
    float CurveEnergyAtPoint(int idx, Point newPoint) const;

    std::tuple<float, float> secondDiff(int index) const;
    std::tuple<float, float> fourthDiff(int index) const;

    Point &operator[](int idx);
    const Point &operator[](int idx) const;
    
    private:
    
    static constexpr double PI = 3.14159265358979323846;
    
    Point m_center;
    int m_numPoints;
    std::vector<Point> m_ContourPoints;
    float m_averagePointSpaceing;
    
    float averagePointDistance(int centerIndex, int width) const;
    float distanceBetweenPoints(const Point &p1, const Point &p2) const;
    void FillEllipseContourPoints(int radius_x, int radius_y);
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_CONTOUR_HPP

