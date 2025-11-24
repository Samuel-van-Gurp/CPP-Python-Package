#include "Contour.hpp"

Contour::Contour(int radius, Point center, int numPoints) // circle contour
    : m_center(center), m_numPoints(numPoints)
{
    FillEllipseContourPoints(radius, radius, center);
}

Contour::Contour(int radius_x, int radius_y, Point center, int numPoints) // ellipse contour
    : m_center(center), m_numPoints(numPoints)
{
    FillEllipseContourPoints(radius_x, radius_y, center);
}

int Contour::Size() const
{
    return m_numPoints;
}

Point *Contour::getContourPoints_ptr()
{
    return m_ContourPoints.data();
}

void Contour::FillEllipseContourPoints(int radius_x, int radius_y,  Point center)
{
    for (int i = 0; i < m_numPoints; ++i)
    {
        double angle = 2.0 * PI * static_cast<double>(i) / static_cast<double>(m_numPoints);
        int x = static_cast<int>(m_center.X + radius_x * cos(angle));
        int y = static_cast<int>(m_center.Y + radius_y * sin(angle));
        m_ContourPoints.emplace_back(x, y);
    }
}

float Contour::TensionEnergyAtPoint(int idx, Point newPoint) const
{
    int left_index = (idx - 1 + m_numPoints) % m_numPoints;
    int right_index = (idx + 1) % m_numPoints;
    Point leftPoint = m_ContourPoints[left_index];
    Point rightPoint = m_ContourPoints[right_index];
    
    float fx = (leftPoint.X - 2 * newPoint.X + rightPoint.X);
    float fy = (leftPoint.Y - 2 * newPoint.Y + rightPoint.Y);
    return (fx * fx) + (fy * fy);
}

float Contour::CurveEnergyAtPoint(int idx, Point newPoint) const
{
    int left_index = (idx - 1 + m_numPoints) % m_numPoints;
    int right_index = (idx + 1) % m_numPoints;

    Point leftPoint = m_ContourPoints[left_index];
    Point rightPoint = m_ContourPoints[right_index];

    float fx = (leftPoint.X - newPoint.X) - (newPoint.X - rightPoint.X);
    float fy = (leftPoint.Y - newPoint.Y) - (newPoint.Y - rightPoint.Y);
    return (fx * fx) + (fy * fy);
}

Point& Contour::operator[](std::size_t idx) { return m_ContourPoints.at(idx); }
const Point& Contour::operator[](std::size_t idx) const { return m_ContourPoints.at(idx); }