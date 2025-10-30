#include "DataObjects/Image.hpp"
#include "DataObjects/Contour.hpp"
#include "IO/WriteImage.hpp"

class SnakeEngine
{
    public:
        SnakeEngine(const Image& image, Contour& contour);

        void RunSnake(int iterations); 

        void EvolveContour();
        Point getNextStep(Point &p);

    private:
        const Image& m_image;
        Contour& m_contour;

};
