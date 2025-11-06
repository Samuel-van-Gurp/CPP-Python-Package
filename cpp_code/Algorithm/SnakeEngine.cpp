#include "SnakeEngine.hpp"

SnakeEngine::SnakeEngine(const ImageProcessor &image, const ImageHolder<uint8_t> &imageHolder, Contour &contour, float alpha, float beta)
    : m_image(image), m_imageHolder(imageHolder), m_contour(contour), alpha(alpha), beta(beta)
{

}

void SnakeEngine::RunSnake(int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        EvolveContour();
    }
}

void SnakeEngine::EvolveContour()
{
    //copy current contour to new contour
    Contour newContour = m_contour;
    // loop over contour points
    int contourSize = m_contour.Size();
    for (int i = 0; i < contourSize; ++i)
    {
        // get current point
        Point& p = m_contour[i];

        // get next step based on image gradient at point location
        Point nextStep = getNextStep(i, p);
        newContour[i] = nextStep;

        // print initial and new point for debugging
        std::cout << "Point " << i << ": (" << p.X << ", " << p.Y << ") -> ("
                  << nextStep.X << ", " << nextStep.Y << ")\n";

    }
    // move new contour to the old contour
    m_contour = std::move(newContour);
}

Point SnakeEngine::getNextStep(int index, Point& p)
{
    // get internal and external energy matrices
    auto InternalEnergyMatrix = constructInternalEnergyMatrix(index, p);
    auto ExternalEnergyMatrix = constructExternalEnergyMatrix(p);

    // combine energies
    std::vector<std::vector<uint8_t>> TotalEnergyMatrix(3, std::vector<uint8_t>(3, 0));
    for (int dy = 0; dy <= 2; ++dy)     
    {
        for (int dx = 0; dx <= 2; ++dx)
        {
            float internalEnergy = InternalEnergyMatrix[dy][dx];
            float externalEnergy = ExternalEnergyMatrix[dy][dx];
            float totalEnergy = internalEnergy + externalEnergy;
            TotalEnergyMatrix[dy][dx] = static_cast<uint8_t>(std::clamp(static_cast<int>(totalEnergy), 0, 255));
        }
    }
    // find minimum energy position
    uint8_t minEnergy = TotalEnergyMatrix[1][1];
    Point bestPoint = p;
    for (int dy = -1; dy <= 1; ++dy)    
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            uint8_t energy = TotalEnergyMatrix[dy + 1][dx + 1];
            if (energy < minEnergy)
            {
                minEnergy = energy;
                bestPoint = Point(p.X + dx, p.Y + dy);
            }
        }
    }

    return bestPoint;
}

std::vector<std::vector<uint8_t>> SnakeEngine::constructExternalEnergyMatrix(Point& p)
{
    // normalise external energy to 0-255
    std::vector<std::vector<uint8_t>> ExternalEnergyMatrix = m_image.getNeighbourhood(p, m_imageHolder);
    ExternalEnergyMatrix = normalizeEnergyMatrix(ExternalEnergyMatrix);

    return ExternalEnergyMatrix;
}



std::vector<std::vector<uint8_t>> SnakeEngine::constructInternalEnergyMatrix(int index, const Point& p)
{
    std::vector<std::vector<float>> InternalEnergyMatrix(3, std::vector<float>(3, 0.0f));

    int idx = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            Point candidatePoint(p.X + dx, p.Y + dy);
            float tension = alpha * m_contour.TensionEnergyAtPoint(index, candidatePoint);
            float curve = beta * m_contour.CurveEnergyAtPoint(index, candidatePoint);
            InternalEnergyMatrix[dy + 1][dx + 1] = tension + curve;
        }
    }

    return normalizeEnergyMatrix(InternalEnergyMatrix);
}
