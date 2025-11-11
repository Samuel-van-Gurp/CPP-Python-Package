#include <gtest/gtest.h>
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Point.hpp"
#include "DataObjects/ImageHolder.hpp"
#include <vector>
#include <cstdint>


ImageProcessor processor;

// Basic checks: width/height/center and GetImageVector dimensions
TEST(ImageProcessorTest, normaliseImageIntensity) 
{
    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
        0.0f, 0.5f, 1.0f, 1.5f,
        2.0f, 2.5f, 3.0f, 3.5f,
        4.0f, 4.5f, 5.0f, 5.5f,
        6.0f, 6.5f, 7.0f, 7.5f
    };
    ImageHolder<float> imageHolder(data, width, height);
    processor.normaliseImageIntensity(imageHolder);

    float maxIntensity = imageHolder.getMaxIntensityInImage();
    float pixelValue11 = imageHolder.getPixel(1,1);

    EXPECT_NEAR(pixelValue11, 0.333333f, 1e-5f);
    EXPECT_NEAR(maxIntensity, 1.0f, 1e-5f);
}

TEST(ImageProcessorTest, GetCoordinateOfMaximumNeighborValueTest) 
{
    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
        0.0f, 0.5f, 1.0f, 1.5f,
        2.0f, 2.5f, 3.0f, 3.5f,
        4.0f, 4.5f, 5.0f, 5.5f,
        6.0f, 6.5f, 7.0f, 7.5f
    };

    ImageHolder<float> imageHolder(data, width, height);
    Point testPoint(2, 2);
    Point maxNeighbor = processor.GetCoordinateOfMaximumNeighborValue(testPoint, imageHolder);

    EXPECT_EQ(maxNeighbor.X, 3);
    EXPECT_EQ(maxNeighbor.Y, 3);
}

TEST(ImageProcessorTest, getNeighbourhoodTest) 
{
    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
        0.0f, 0.5f, 1.0f, 1.5f,
        2.0f, 2.5f, 3.0f, 3.5f,
        4.0f, 4.5f, 5.0f, 5.5f,
        6.0f, 6.5f, 7.0f, 7.5f
    };

    ImageHolder<float> imageHolder(data, width, height);
    Point testPoint(1, 1);
    auto neighborhood = processor.getNeighbourhood(testPoint, imageHolder);

    std::vector<std::vector<float>> expectedNeighborhood = {
        {0.0f, 0.5f, 1.0f},
        {2.0f, 2.5f, 3.0f},
        {4.0f, 4.5f, 5.0f}
    };

    EXPECT_EQ(neighborhood, expectedNeighborhood);
}

TEST(ImageProcessorTest, ConvolveImage) 
{

    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f
    };

    ImageHolder<float> imageHolder(data, width, height);

    std::vector<std::vector<float>> kernel = {
        {0.0f, 0.2f, 0.0f},
        {0.2f, 0.2f, 0.2f},
        {0.0f, 0.2f, 0.0f}
    };

    ImageHolder<float> convolvedImage = processor.ConvolveImage(kernel, imageHolder);

    float centerPixelValue = convolvedImage.getPixel(2, 1);
    EXPECT_NEAR(centerPixelValue, 0.2f, 1e-5f);
}


TEST(ImageProcessorTest, ComputeGradientMagnitude) 
{
    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f
    };

    ImageHolder<float> imageHolder(data, width, height);

    ImageHolder<float> gradientImage = processor.ComputeGradientMagnitude(imageHolder);

    float centerPixelValue = gradientImage.getPixel(1, 1);
    float otherPixelValue = gradientImage.getPixel(1, 2);
    EXPECT_NEAR(centerPixelValue, 0.0f, 1e-5f);
    EXPECT_NEAR(otherPixelValue, 2.0f, 1e-5f);
}

TEST(ImageProcessorTest, scaleIntensity) 
{
    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
        0.0f, 0.5f, 0.0f, 1.5f,
        2.0f, 2.5f, 3.0f, 3.5f,
        4.0f, 4.5f, 5.0f, 5.5f,
        6.0f, 6.5f, 7.0f, 7.5f
    };
    ImageHolder<float> imageHolder(data, width, height);
    processor.scaleIntensity(2, imageHolder);

    float pixelValue11 = imageHolder.getPixel(1,1);
    float pixelValue33 = imageHolder.getPixel(3,3);

    // print pixel values for debugging
    EXPECT_NEAR(pixelValue11, 1.25f, 1e-5f);
    EXPECT_NEAR(pixelValue33, 3.75f, 1e-5f);
}

TEST(ImageProcessorTest, invertImageIntensity)
{
    // construct dummy imageHolder from a vector
    int width = 4;
    int height = 4;
    std::vector<float> data = {
        0.0f, 0.2f, 0.4f, 0.6f,
        0.8f, 1.0f, 0.5f, 0.3f,
        0.7f, 0.9f, 0.1f, 0.05f,
        0.15f, 0.25f, 0.35f, 0.45f
    };
    ImageHolder<float> imageHolder(data, width, height);
    processor.invertImageIntensity(imageHolder);

    float pixelValue00 = imageHolder.getPixel(0,0);
    float pixelValue11 = imageHolder.getPixel(1,1);
    float pixelValue23 = imageHolder.getPixel(3,2);

    EXPECT_NEAR(pixelValue00, 1.0f, 1e-5f);
    EXPECT_NEAR(pixelValue11, 0.0f, 1e-5f);
    EXPECT_NEAR(pixelValue23, 0.95f, 1e-5f);
}

// TEST(ImageProcessorTest, GetMaxIntensityInImage)