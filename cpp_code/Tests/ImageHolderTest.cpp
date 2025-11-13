#include <gtest/gtest.h>
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "ImageProcessing/ImageProcessor.hpp"
#include "DataObjects/ImageHolder.hpp"


TEST(test_ImageHolder, getMaxIntensityInImage) 
{
    // construct dummy imageHolder from a vector
    int width = 3;
    int height = 3;
    std::vector<float> data = {
        0.1f, 0.5f, 0.3f,
        0.7f, -0.2f, 0.9f,
        0.4f, 0.6f, 0.8f
    };

    ImageHolder<float> imageHolder(data, width, height);
    float maxIntensity = imageHolder.getMaxIntensityInImage();

    EXPECT_NEAR(maxIntensity, 0.9f, 1e-5f);
}

TEST(test_ImageHolder, setAndGetPixel) 
{
    // construct dummy imageHolder from a vector
    int width = 2;
    int height = 2;
    std::vector<float> data = {
        0.0f, 0.0f,
        0.0f, 0.0f
    };

    ImageHolder<float> imageHolder(data, width, height);
    imageHolder.setPixel(1, 0, 0.75f); // set pixel at (1,0) to 0.75

    float pixelValue = imageHolder.getPixel(1, 0);
    EXPECT_NEAR(pixelValue, 0.75f, 1e-5f);
}

