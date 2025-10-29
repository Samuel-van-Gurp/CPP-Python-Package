#include "Point.hpp"
#include <vector>
#include <cmath>


class Contour
{
    public:
    Contour(int radius, Point center, int numPoints); 
    
    private:
    static constexpr double M_PI = 3.14159265358979323846;
    
    int m_radius;
    Point m_center;
    int m_numPoints;
    std::vector<Point> m_ContourPoints;

    void FillContourPoints();
};
