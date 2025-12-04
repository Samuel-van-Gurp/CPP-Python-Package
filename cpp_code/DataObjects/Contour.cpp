#include "Contour.hpp"
#include <iostream>
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

std::tuple<float, float> Contour::secondDiff(int index)
{
    float h = 1.0f;
    
    float dx2 = (1.0f/(h*h)) * ((*this)[index+1].X - 2.0f*(*this)[index].X + (*this)[index-1].X);
    float dy2 = (1.0f/(h*h)) * ((*this)[index+1].Y - 2.0f*(*this)[index].Y + (*this)[index-1].Y);
    
    return std::make_tuple(dx2, dy2);
}

std::tuple<float, float> Contour::fourthDiff(int index){
    float h = 1.0f;
    float inv_h4 = 1.0f / (h * h * h * h);
    float dx4 = inv_h4 * ((*this)[index-2].X - 4.0f * (*this)[index-1].X + 6.0f * (*this)[index].X - 4.0f * (*this)[index+1].X + (*this)[index+2].X);
    float dy4 = inv_h4 * ((*this)[index-2].Y - 4.0f * (*this)[index-1].Y + 6.0f * (*this)[index].Y - 4.0f * (*this)[index+1].Y + (*this)[index+2].Y);

    return std::make_tuple(dx4, dy4);
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