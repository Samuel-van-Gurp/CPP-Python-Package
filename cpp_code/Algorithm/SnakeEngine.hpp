#include "DataObjects/Image.hpp"
#include "DataObjects/Contour.hpp"
#include "IO/WriteImage.hpp"

class SnakeEngine
{
public:

        SnakeEngine(const Image &image, Contour &contour, float alpha, float beta);

        void RunSnake(int iterations);

        void EvolveContour();
        Point getNextStep(Point &p);
        Point constructEnergyMatrix();

        
    private:

        const float alpha;    
        const float beta;

        std::vector<std::vector<uint8_t>> constructTotalEnergyMatrix(Point& p);
        std::vector<std::vector<uint8_t>> constructInternalEnergyMatrix(Point &p);
        const Image &m_image;
        Contour &m_contour;
};
