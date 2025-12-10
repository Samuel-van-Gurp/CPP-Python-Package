#include <gtest/gtest.h>
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"

TEST(ContourTest, point_init_Test) {
    Contour contour(2, Point(0,0), 4);
    EXPECT_NEAR(contour[0].X,  2.0f, 1e-5f);
    EXPECT_NEAR(contour[0].Y,  0.0f, 1e-5f);
    EXPECT_NEAR(contour[1].X,  0.0f, 1e-5f);
    EXPECT_NEAR(contour[1].Y,  2.0f, 1e-5f);
    EXPECT_NEAR(contour[2].X, -2.0f, 1e-5f);
    EXPECT_NEAR(contour[2].Y,  0.0f, 1e-5f);
    EXPECT_NEAR(contour[3].X,  0.0f, 1e-5f);
    EXPECT_NEAR(contour[3].Y, -2.0f, 1e-5f);
}

TEST(ContourTest, ContourTest_TentionEner_Test) 
{
    Contour contour(4, Point(0,0), 40);
    Point newPoint{1.0f, 1.0f};
    
    float tensionEnergy = contour.TensionEnergyAtPoint(0, newPoint);
    EXPECT_NEAR(tensionEnergy, 18.0f, 1e-5f);
}