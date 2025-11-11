#include <cstdint>
#include <cassert>
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Contour.hpp"
#include "Algorithm/SnakeEngine.hpp"
#include "IO/WriteImage.hpp"
#include "DataObjects/ImageHolder.hpp"

class SnakeInterface
{
    
public:
    SnakeInterface(const uint8_t *data, int width, int height, int stride, float alpha, float beta);
    void run(int iterations);

private:

    ImageHolder<float> m_ImageHolder;
    ImageProcessor ImageProcessor;
    Contour m_contour;
    SnakeEngine m_engine;
    WriteImage m_writer;
};
 