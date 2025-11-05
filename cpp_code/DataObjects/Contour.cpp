#include "Contour.hpp"

Contour::Contour(int radius, Point center, int numPoints)
    : m_radius(radius), m_center(center), m_numPoints(numPoints)
{
    FillContourPoints();
}

int Contour::Size() const
{
    return m_numPoints;
}

void Contour::FillContourPoints()
{
    for (int i = 0; i < m_numPoints; ++i)
    {
        double angle = 2.0 * PI * static_cast<double>(i) / static_cast<double>(m_numPoints);
        int x = static_cast<int>(m_center.X + m_radius * cos(angle));
        int y = static_cast<int>(m_center.Y + m_radius * sin(angle));
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
    int far_left_index = (idx - 2 + m_numPoints) % m_numPoints;
    int far_right_index = (idx + 2) % m_numPoints;
    Point leftPoint = m_ContourPoints[left_index];
    Point rightPoint = m_ContourPoints[right_index];
    Point farLeftPoint = m_ContourPoints[far_left_index];
    Point farRightPoint = m_ContourPoints[far_right_index];

    float fx = (farLeftPoint.X - 4 * leftPoint.X + 6 * newPoint.X - 4 * rightPoint.X + farRightPoint.X);
    float fy = (farLeftPoint.Y - 4 * leftPoint.Y + 6 * newPoint.Y - 4 * rightPoint.Y + farRightPoint.Y);

    return (fx * fx) + (fy * fy);
}

Point& Contour::operator[](std::size_t idx) { return m_ContourPoints.at(idx); }
const Point& Contour::operator[](std::size_t idx) const { return m_ContourPoints.at(idx); }