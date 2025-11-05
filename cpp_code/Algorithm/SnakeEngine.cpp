#include "SnakeEngine.hpp"

SnakeEngine::SnakeEngine(const Image &image, Contour &contour, float alpha, float beta)
    : m_image(image), m_contour(contour), alpha(alpha), beta(beta)  
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
        Point nextStep = getNextStep(p);
        newContour[i] = nextStep;

        // print initial and new point for debugging
        // std::cout << "Point " << i << ": (" << p.X << ", " << p.Y << ") -> ("
        //           << nextStep.X << ", " << nextStep.Y << ")\n";

    }
    // move new contour to the old contour
    m_contour = std::move(newContour);
}

Point SnakeEngine::getNextStep(Point& p)
{
    // get internal and external energy matrices
    auto InternalEnergyMatrix = constructInternalEnergyMatrix(p);
    auto ExternalEnergyMatrix = constructTotalEnergyMatrix(p);

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
    uint8_t minEnergy = 255;
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

std::vector<std::vector<uint8_t>> SnakeEngine::constructTotalEnergyMatrix(Point& p)
{
    std::vector<std::vector<uint8_t>> ExternalEnergyMatrix = m_image.getNeighbourhood(p);

    return ExternalEnergyMatrix;
}

std::vector<std::vector<uint8_t>> SnakeEngine::constructInternalEnergyMatrix(Point& p)
{
    std::vector<std::vector<uint8_t>> InternalEnergyMatrix;
    InternalEnergyMatrix.resize(3, std::vector<uint8_t>(3, 0));

    // Collect all energy values to find min/max
    std::vector<float> energies;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            float tension = alpha * m_contour.TensionEnergyAtPoint(0, p);
            float curve = beta * m_contour.CurveEnergyAtPoint(0, p);
            // std::cout << "Tension: " << tension << ", Curve: " << curve << "\n";
            energies.push_back(tension + curve);
        }
    }

    // Find min and max
    float minEnergy = *std::min_element(energies.begin(), energies.end());
    float maxEnergy = *std::max_element(energies.begin(), energies.end());
    float range = maxEnergy - minEnergy;
    if (range == 0) range = 1; // avoid division by zero

    // Fill matrix with normalized values
    int idx = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            float energy = energies[idx++];
            uint8_t norm = static_cast<uint8_t>(255.0f * (energy - minEnergy) / range);
            InternalEnergyMatrix[dy + 1][dx + 1] = norm;
        }
    }

    return InternalEnergyMatrix;
}
