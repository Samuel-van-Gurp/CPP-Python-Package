#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class Image
{
private:
   const uint8_t *m_data;
    int m_width;
    int m_height;
    int m_stride;
    std::vector<std::vector<uint8_t>> m_image;
public:
    Image(const uint8_t *data, int width, int height, int stride);
    // ~Image();
    

    // convert the image back to vector of vectors
    void ReconstructImage();
    void SaveAsPGM();
};
