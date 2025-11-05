#include "DataObjects/Image.hpp"
#include "DataObjects/Contour.hpp"
#include "IO/WriteImage.hpp"

class SnakeEngine
{
public:

        SnakeEngine(const Image &image, Contour &contour, float alpha, float beta);

        void RunSnake(int iterations);

        void EvolveContour();
        Point getNextStep(int index, Point &p);
        Point constructEnergyMatrix();

        
    private:

        const float alpha;    
        const float beta;

        std::vector<std::vector<uint8_t>> constructExternalEnergyMatrix(Point& p);
        std::vector<std::vector<uint8_t>> constructInternalEnergyMatrix(int index, const Point &p);

        // Helper function to normalize a 3x3 matrix of float energies to uint8_t (0-255)
        template <typename T>
        std::vector<std::vector<uint8_t>> normalizeEnergyMatrix(const std::vector<std::vector<T>>& energyMatrix)
        {
            std::vector<uint8_t> flatEnergies;
            for (const auto& row : energyMatrix)
                for (float val : row)
                    flatEnergies.push_back(val);

            float minEnergy = *std::min_element(flatEnergies.begin(), flatEnergies.end());
            float maxEnergy = *std::max_element(flatEnergies.begin(), flatEnergies.end());
            float range = maxEnergy - minEnergy;
            if (range == 0) range = 1;

            std::vector<std::vector<uint8_t>> normMatrix(3, std::vector<uint8_t>(3, 0));
            int idx = 0;
            for (int dy = 0; dy < 3; ++dy)
            {
                for (int dx = 0; dx < 3; ++dx)
                {
                    T energy = energyMatrix[dy][dx];
                    normMatrix[dy][dx] = static_cast<uint8_t>(255.0f * (energy - minEnergy) / range);
                }
            }
            return normMatrix;
        }

        const Image &m_image;
        Contour &m_contour;
};
