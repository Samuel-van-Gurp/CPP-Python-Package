#include "SnakeEngine.hpp"

SnakeEngine::SnakeEngine(const Image &image, Contour &contour)
    : m_image(image), m_contour(contour)
{

}

void SnakeEngine::RunSnake(int iterations)
{
    // loop
    for (int i = 0; i < iterations; ++i)
    {
        EvolveContour();
    }

}

void SnakeEngine::EvolveContour()
{
    // loop over contour points
    int contourSize = m_contour.Size();
    for (int i = 0; i < contourSize; ++i)
    {
        // get current point
        Point& p = m_contour[i];

        // get next step based on image gradient at point location
        Point nextStep = getNextStep(p);
        m_contour[i] = nextStep;

        // print initial and new point for debugging
        std::cout << "Point " << i << ": (" << p.X << ", " << p.Y << ") -> ("
                  << nextStep.X << ", " << nextStep.Y << ")\n";

    }
}

Point SnakeEngine::getNextStep(Point& p)
{
    // get the highest value direction around point p (8 connectivity)
    Point nextPoint = m_image.GetCoordinateOfHighestValueDirection(p);
    return nextPoint;
}
