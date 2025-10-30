#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP

#include "Contour.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class Image
{
public:    
    Image(const uint8_t *data, int width, int height, int stride);
    std::vector<std::vector<uint8_t>> scaleIntensity(int factor) const;
    void normaliseImageIntensity();

    // getters and setters for image properties
    int GetWidth() const;
    int GetHeight() const;
    const std::vector<std::vector<uint8_t>>& GetImageVector() const;
    Point GetCenter() const;

private:
    int m_width;
    int m_height;
    int m_stride;
    std::vector<std::vector<uint8_t>> m_image;
    void ReconstructImage(const uint8_t *data);
    void ConstructImageGradient();
    void BlurImage(int blurAmount);
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP
