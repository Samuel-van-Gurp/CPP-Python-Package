#include <cstdint>
#include "DataObjects/Image.hpp"
#include "DataObjects/Contour.hpp"
#include "Algorithm/SnakeEngine.hpp"
#include "IO/WriteImage.hpp"

class SnakeInterface
{
    
public:
    SnakeInterface(const uint8_t *data, int width, int height, int stride);
private:
};
