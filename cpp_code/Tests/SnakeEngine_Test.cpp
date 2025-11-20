#include <gtest/gtest.h>
#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.cpp"
#include "Algorithm/GreedySnakeEngine.hpp"
#include <vector>

// TEST(SnakeEngineTest, ContourEvolvesAndStaysInBounds) {
//     // create a small synthetic image with a bright center to attract the snake
//     const int w = 11;
//     const int h = 11;
//     std::vector<uint8_t> data(w * h, 0);
//     for (int y = 0; y < h; ++y) {
//         for (int x = 0; x < w; ++x) {
//             int dx = x - w/2;
//             int dy = y - h/2;
//             int val = std::max(0, 255 - (std::abs(dx) + std::abs(dy)) * 50);
//             data[y * w + x] = static_cast<uint8_t>(val);
//         }
//     }

//     Image img(data.data(), w, h, w);
//     Contour contour(3, Point(w/2, h/2), 8);

//     // record original contour
//     std::vector<Point> before;
//     for (int i = 0; i < contour.Size(); ++i) before.push_back(contour[i]);

//     SnakeEngine engine(img, contour, 0.5f, 0.5f);
//     engine.RunSnake(5);

//     // at least one point should have moved
//     bool moved = false;
//     for (int i = 0; i < contour.Size(); ++i) {
//         if (contour[i].X != before[i].X || contour[i].Y != before[i].Y) {
//             moved = true;
//             break;
//         }
//     }
//     EXPECT_TRUE(moved);

//     // all points must remain inside image bounds
//     for (int i = 0; i < contour.Size(); ++i) {
//         EXPECT_GE(contour[i].X, 0);
//         EXPECT_LT(contour[i].X, w);
//         EXPECT_GE(contour[i].Y, 0);
//         EXPECT_LT(contour[i].Y, h);
//     }
// }