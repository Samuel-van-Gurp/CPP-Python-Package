#include "Contour.hpp"
#include <iostream>

Contour::Contour(int numPoints) // empty contour
    : m_center(Point{0.0f, 0.0f}), m_numPoints(numPoints)
{
    m_ContourPoints.resize(static_cast<std::size_t>(m_numPoints), Point{0.0f, 0.0f});
}


Contour::Contour(int radius, Point center, int numPoints) // circle contour
    : m_center(center), m_numPoints(numPoints)
{
    FillEllipseContourPoints(radius, radius);
}

Contour::Contour(int radius_x, int radius_y, Point center, int numPoints) // ellipse contour
    : m_center(center), m_numPoints(numPoints)
{
    FillEllipseContourPoints(radius_x, radius_y);
}

int Contour::Size() const
{
    return m_numPoints;
}

std::vector<Point> Contour::getContourPoints()
{
    return m_ContourPoints;
}

void Contour::FillEllipseContourPoints(int radius_x, int radius_y)
{
    for (int i = 0; i < m_numPoints; ++i)
    {
        double angle = 2.0 * PI * static_cast<double>(i) / static_cast<double>(m_numPoints);
        float x = m_center.X + radius_x * cos(angle);
        float y = m_center.Y + radius_y * sin(angle);

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

std::tuple<float, float> Contour::secondDiff(int index) const
{
    float h1 = distanceBetweenPoints((*this)[index+1], (*this)[index]);
    float h2 = distanceBetweenPoints((*this)[index], (*this)[index-1]);

    float dx2 = 2 * ( ((*this)[index-1].X)/(h2*(h1 + h2)) - (*this)[index].X / (h1 * h2) + (*this)[index+1].X / (h1 * (h1 + h2)));
    float dy2 = 2 * ( ((*this)[index-1].Y)/(h2*(h1 + h2)) - (*this)[index].Y / (h1 * h2) + (*this)[index+1].Y / (h1 * (h1 + h2)));

    return std::make_tuple(dx2, dy2);
}

std::tuple<float, float> Contour::fourthDiff(int index) const
{
    float h = distanceBetweenPoints((*this)[index+1], (*this)[index]); // assuming uniform sampling
    float inv_h4 = 1.0f / (h * h * h * h);
    float dx4 = inv_h4 * ((*this)[index-2].X - 4.0f * (*this)[index-1].X + 6.0f * (*this)[index].X - 4.0f * (*this)[index+1].X + (*this)[index+2].X);
    float dy4 = inv_h4 * ((*this)[index-2].Y - 4.0f * (*this)[index-1].Y + 6.0f * (*this)[index].Y - 4.0f * (*this)[index+1].Y + (*this)[index+2].Y);

    return std::make_tuple(dx4, dy4);
}

float Contour::distanceBetweenPoints(const Point& p1, const Point& p2) const
{
    float dx = p2.X - p1.X;
    float dy = p2.Y - p1.Y;
    return std::sqrt(dx * dx + dy * dy);
}

Point& Contour::operator[](int idx) 
{
    int wrapped_idx = idx % Size();
    if (wrapped_idx < 0) 
    {
        wrapped_idx += Size();
    }
    return m_ContourPoints.at(static_cast<std::size_t>(wrapped_idx)); 
}

const Point& Contour::operator[](int idx) const 
{
    int wrapped_idx = idx % Size();
    if (wrapped_idx < 0) 
    {
        wrapped_idx += Size();
    }
    return m_ContourPoints.at(static_cast<std::size_t>(wrapped_idx)); 
}