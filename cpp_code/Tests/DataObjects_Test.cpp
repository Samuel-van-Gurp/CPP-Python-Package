#include <gtest/gtest.h>
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"

TEST(ContourTest, point_init_Test) {
    Contour contour(2, Point(0,0), 4);
    EXPECT_EQ(contour[0].X, 2);
    EXPECT_EQ(contour[0].Y, 0);
    EXPECT_EQ(contour[1].X, 0);
    EXPECT_EQ(contour[1].Y, 2);
    EXPECT_EQ(contour[2].X, -2);
    EXPECT_EQ(contour[2].Y, 0);
    EXPECT_EQ(contour[3].X, 0);
    EXPECT_EQ(contour[3].Y, -2);
}
