#include <gtest/gtest.h>
#include "DataObjects/Image.hpp"
#include "DataObjects/Point.hpp"
#include <vector>
#include <cstdint>

// Basic checks: width/height/center and GetImageVector dimensions
TEST(ImageTest, BasicProperties) {
    int w = 4, h = 3, stride = 4;
    std::vector<uint8_t> data(w * h);
    for (int i = 0; i < w * h; ++i) data[i] = static_cast<uint8_t>(i % 256);

    Image img(data.data(), w, h, stride);

    EXPECT_EQ(img.GetWidth(), w);
    EXPECT_EQ(img.GetHeight(), h);

    Point c = img.GetCenter();
    EXPECT_EQ(c.X, w / 2);
    EXPECT_EQ(c.Y, h / 2);

    const auto &vec = img.GetImageVector();
    EXPECT_EQ(static_cast<int>(vec.size()), h);
    EXPECT_EQ(static_cast<int>(vec[0].size()), w);
}

// scaleIntensity should divide each pixel by factor (integer division)
TEST(ImageTest, ScaleIntensity) {
    int w = 5, h = 5, stride = 5;
    std::vector<uint8_t> data(w * h);
    for (int i = 0; i < w * h; ++i) data[i] = static_cast<uint8_t>((i * 7) % 256);

    Image img(data.data(), w, h, stride);
    const auto &orig = img.GetImageVector();
    auto scaled = img.scaleIntensity(2);

    ASSERT_EQ(static_cast<int>(scaled.size()), h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            uint8_t expected = static_cast<uint8_t>(orig[y][x] / 2);
            EXPECT_EQ(scaled[y][x], expected) << "at (" << x << "," << y << ")";
        }
    }
}

// getNeighbourhood should return a 3x3 matrix and the center element matches the image pixel
TEST(ImageTest, NeighbourhoodCenterAndBorder) {
    int w = 5, h = 5, stride = 5;
    std::vector<uint8_t> data(w * h, 0);
    // make a simple pattern
    data[2 * w + 2] = 128;
    data[0] = 10;

    Image img(data.data(), w, h, stride);
    const auto &vec = img.GetImageVector();

    Point center(2, 2);
    auto neigh = img.getNeighbourhood(center);
    ASSERT_EQ(neigh.size(), 3);
    ASSERT_EQ(neigh[1].size(), 3);
    EXPECT_EQ(neigh[1][1], vec[2][2]);

    Point corner(0, 0);
    auto neigh0 = img.getNeighbourhood(corner);
    EXPECT_EQ(neigh0[0][0], vec[0][0]);
}
