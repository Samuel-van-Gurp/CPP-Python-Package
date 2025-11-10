#ifndef SNAKEENGINE_HPP
#define SNAKEENGINE_HPP
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Contour.hpp"
#include "IO/WriteImage.hpp"

class SnakeEngine
{
public:

        SnakeEngine(const ImageProcessor &image, const ImageHolder<float> &imageHolder, Contour &contour, float alpha, float beta);

        void RunSnake(int iterations);

        void EvolveContour();
        Point getNextStep(int index, Point &p);
        Point constructEnergyMatrix();

        
    private:

        const float alpha;    
        const float beta;

        std::vector<std::vector<float>> constructExternalEnergyMatrix(Point& p);
        std::vector<std::vector<float>> constructInternalEnergyMatrix(int index, const Point &p);

        std::vector<std::vector<float>> combineEnergyMatrix(const std::vector<std::vector<float>> &EnergyMatrix1, const std::vector<std::vector<float>> &EnergyMatrix2, float weight1, float weight2);

        // Helper function to normalize a 3x3 matrix of float energies to uint8_t (0-255)
        template <typename T>
        std::vector<std::vector<float>> normalizeEnergyMatrix(const std::vector<std::vector<T>>& energyMatrix)
        {
            std::vector<float> flatEnergies;
            for (const auto& row : energyMatrix)
                for (float val : row)
                    flatEnergies.push_back(val);

            float minEnergy = *std::min_element(flatEnergies.begin(), flatEnergies.end());
            float maxEnergy = *std::max_element(flatEnergies.begin(), flatEnergies.end());
            float range = maxEnergy - minEnergy;
            if (range == 0) range = 1;

            std::vector<std::vector<float>> normMatrix(3, std::vector<float>(3, 0));
            int idx = 0;
            for (int dy = 0; dy < 3; ++dy)
            {
                for (int dx = 0; dx < 3; ++dx)
                {
                    T energy = energyMatrix[dy][dx];
                    normMatrix[dy][dx] = static_cast<float>((energy - minEnergy) / range);
                }
            }
            return normMatrix;
        }

        const ImageProcessor &m_image;
        const ImageHolder<float> &m_imageHolder;
        Contour &m_contour;
};
#endif // SNAKEENGINE_HPP