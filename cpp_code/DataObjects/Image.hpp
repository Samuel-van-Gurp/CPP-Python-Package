#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

class Image
{
private:
   const uint8_t *m_data;
    int m_width;
    int m_height;
    int m_stride;
    std::vector<std::vector<uint8_t>> m_image;
    void ReconstructImage();
    void ConstructImageGradient();
    void BlurImage(int blurAmount);

public:
    
    Image(const uint8_t *data, int width, int height, int stride);
    void normaliseImageIntensity();
    void SaveAsPGM();
};
