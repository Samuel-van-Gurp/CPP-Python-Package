#include "contour.hpp"

Contour::Contour(int radius, Point center, int numPoints)
    : m_radius(radius), m_center(center), m_numPoints(numPoints)
{
    FillContourPoints();
}

void Contour::FillContourPoints()
{
    for (int i = 0; i < m_numPoints; ++i)
    {
        double angle = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(m_numPoints);
        int x = static_cast<int>(m_center.X + m_radius * cos(angle));
        int y = static_cast<int>(m_center.Y + m_radius * sin(angle));
        m_ContourPoints.emplace_back(x, y);
    }
}