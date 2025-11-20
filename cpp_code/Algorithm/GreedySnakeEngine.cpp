#include "GreedySnakeEngine.hpp"

GreedySnakeEngine::GreedySnakeEngine(const ImageProcessorFacade &image, const ImageHolder<float> &imageHolder, Contour &contour, float alpha, float beta)
    : m_image(image), m_imageHolder(imageHolder), m_contour(contour), alpha(alpha), beta(beta)
{

}

Point* GreedySnakeEngine::RunSnake(int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        EvolveContour();
    }

    return m_contour.getContourPoints_ptr();
}

void GreedySnakeEngine::EvolveContour()
{
    //copy current contour to new contour
    Contour newContour = m_contour;

    int contourSize = m_contour.Size();
    for (int i = 0; i < contourSize; ++i)
    {
        Point& p = m_contour[i];

        Point nextStep = getNextStep(i, p);
        newContour[i] = nextStep;

        std::cout << "Point " << i << ": (" << p.X << ", " << p.Y << ") -> ("
                  << nextStep.X << ", " << nextStep.Y << ")\n";

    }
    // move new contour to the old contour
    m_contour = std::move(newContour);
}

Point GreedySnakeEngine::getNextStep(int index, Point& p)
{
    // get internal and external energy matrices
    auto InternalEnergyMatrix = constructInternalEnergyMatrix(index, p);
    auto ExternalEnergyMatrix = constructExternalEnergyMatrix(p);

    // combine energies
    std::vector<std::vector<float>> TotalEnergyMatrix(3, std::vector<float>(3, 0));
    
    TotalEnergyMatrix = combineEnergyMatrix(InternalEnergyMatrix, ExternalEnergyMatrix, 1, 1);

    // find minimum energy position
    float minEnergy = TotalEnergyMatrix[1][1];
    Point bestPoint = p;
    for (int dy = -1; dy <= 1; ++dy)    
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            float energy = TotalEnergyMatrix[dy + 1][dx + 1];
            if (energy < minEnergy)
            {
                minEnergy = energy;
                bestPoint = Point(p.X + dx, p.Y + dy);
            }
        }
    }

    return bestPoint;
}

std::vector<std::vector<float>> GreedySnakeEngine::constructExternalEnergyMatrix(Point& p)
{
    // normalise external energy to [0,1]
    std::vector<std::vector<float>> ExternalEnergyMatrix = m_imageHolder.getNeighbourhood(p);
    ExternalEnergyMatrix = normalizeEnergyMatrix(ExternalEnergyMatrix);

    return ExternalEnergyMatrix;
}

std::vector<std::vector<float>> GreedySnakeEngine::constructInternalEnergyMatrix(int index, const Point& p)
{
    std::vector<std::vector<float>> TentionEnergyMatrix(3, std::vector<float>(3, 0.0f));
    std::vector<std::vector<float>> CurveEnergyMatrix(3, std::vector<float>(3, 0.0f));

    int idx = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            Point candidatePoint(p.X + dx, p.Y + dy);
            float tension = alpha * m_contour.TensionEnergyAtPoint(index, candidatePoint);
            float curve = beta * m_contour.CurveEnergyAtPoint(index, candidatePoint);
            TentionEnergyMatrix[dy + 1][dx + 1] = tension;
            CurveEnergyMatrix[dy + 1][dx + 1] = curve;
        }
    }

    // normalise the energy matrices
    TentionEnergyMatrix = normalizeEnergyMatrix(TentionEnergyMatrix);
    CurveEnergyMatrix = normalizeEnergyMatrix(CurveEnergyMatrix);

    // combine tension and curve energy into internal energy matrix
    return combineEnergyMatrix(TentionEnergyMatrix, CurveEnergyMatrix, alpha, beta);
}

std::vector<std::vector<float>> GreedySnakeEngine::combineEnergyMatrix(const std::vector<std::vector<float>>& EnergyMatrix1, const std::vector<std::vector<float>>& EnergyMatrix2, float weight1, float weight2)
{
    // calculate wighted sum of two energy matrices
    std::vector<std::vector<float>> CombinedMatrix(3, std::vector<float>(3, 0.0f));

    for (int dy = 0; dy < 3; ++dy)
    {
        for (int dx = 0; dx < 3; ++dx)
        {
            CombinedMatrix[dy][dx] = weight1 * EnergyMatrix1[dy][dx] + weight2 * EnergyMatrix2[dy][dx];
        }
    }
    return CombinedMatrix;
}
