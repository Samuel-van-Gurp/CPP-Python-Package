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
    float h1 = distanceBetweenPoints((*this)[index+1], (*this)[index]);
    float h2 = distanceBetweenPoints((*this)[index], (*this)[index-1]);

    float dx2 = 2 * ( ((*this)[index-1].X)/(h2*(h1 + h2)) - (*this)[index].X / (h1 * h2) + (*this)[index+1].X / (h1 * (h1 + h2)));
    float dy2 = 2 * ( ((*this)[index-1].Y)/(h2*(h1 + h2)) - (*this)[index].Y / (h1 * h2) + (*this)[index+1].Y / (h1 * (h1 + h2)));

    return std::make_tuple(dx2, dy2);
}

std::tuple<float, float> Contour::fourthDiff(int index)
{
    float h1 = distanceBetweenPoints((*this)[index-2], (*this)[index-1]);
    float h2 = distanceBetweenPoints((*this)[index-1], (*this)[index]);
    float h3 = distanceBetweenPoints((*this)[index], (*this)[index+1]);
    float h4 = distanceBetweenPoints((*this)[index+1], (*this)[index+2]);
    
    float A_minus2 = 24.0f / (h1 * (h1 + h2) * (h1 + h2 + h3) * (h1 + h2 + h3 + h4));
    float A_minus1 = 24.0f / (h1 * h2 * (h2 + h3) * (h2 + h3 + h4)); 
    float A_0      = 24.0f / ( (h1+h2) * h2 * h3 * (h3+h4) );
    float A_plus1  = 24.0f / ((h1+h2+h3) * (h2+h3) * h3 * h4);
    float A_plus2  = 24.0f / (h4 * (h3 + h4) * (h2 + h3 + h4) * (h1 + h2 + h3 + h4));

    float dx4 = A_minus2 * (*this)[index-2].X - A_minus1 * (*this)[index-1].X + A_0 * (*this)[index].X - A_plus1 * (*this)[index+1].X + A_plus2 * (*this)[index+2].X;
    float dy4 = A_minus2 * (*this)[index-2].Y - A_minus1 * (*this)[index-1].Y + A_0 * (*this)[index].Y - A_plus1 * (*this)[index+1].Y + A_plus2 * (*this)[index+2].Y;

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