#include <gtest/gtest.h>
#include "Algorithm/ELSnakeEngine.hpp"
#include "DataObjects/Contour.hpp"
#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "ImageProcessing/NaiveConvolve.hpp"    
#include "ImageProcessing/IntensityManipulator.hpp"

TEST(ELSnakeEngineTest, combineForces) {
    std::vector<float> data = {
        0.0f, 0.5f, 1.0f, 1.5f,
        2.0f, 2.5f, 3.0f, 3.5f,
        4.0f, 4.5f, 5.0f, 5.5f,
        6.0f, 6.5f, 7.0f, 7.5f
    };
    ImageHolder<float> imageHolder(data, 4, 4);
    Contour contour(1, Point(2,2), 5);

    auto convolver = std::make_unique<NaiveConvolve>();
    auto intensityManipulator = std::make_unique<IntensityManipulator>();
    ImageProcessorFacade imageProcessor(std::move(convolver), std::move(intensityManipulator));
    auto gradients = imageProcessor.PrepareImageForELSnake(imageHolder);

    ELSnakeEngine snakeEngine(gradients, 0.5f, 0.5f);


    std::tuple<float, float> internalForce = std::make_tuple(2.0f, 4.0f);
    std::tuple<float, float> externalForce = std::make_tuple(6.0f, 8.0f);

    auto combinedForce = snakeEngine.combineForces(internalForce, externalForce);

    EXPECT_FLOAT_EQ(std::get<0>(combinedForce), 8.0f);
    EXPECT_FLOAT_EQ(std::get<1>(combinedForce), 12.0f); 
}

TEST(ELSnakeEngineTest, secondDiff) {
    Contour contour(5, Point(0,0), 5);
    
    // over write contour points 
    contour[0] = Point(0.0f, 0.0f);
    contour[1] = Point(1.0f, 1.0f);
    contour[2] = Point(2.0f, 0.0f);
    contour[3] = Point(1.0f, -1.0f);
    contour[4] = Point(0.0f, 0.0f);

    auto [dx2, dy2] = contour.secondDiff(2);
  
    EXPECT_FLOAT_EQ(dx2, -2.0f); // 1 - (2*2) + 1 = -2
    EXPECT_FLOAT_EQ(dy2, -0.0f); //  -1 - (2*0) + 1 = 0
}

TEST(ELSnakeEngineTest, fourthDiff) {
    Contour contour(5, Point(0,0), 5);
    
    // over write contour points 
    contour[0] = Point(0.0f, 0.0f);
    contour[1] = Point(1.0f, 1.0f);
    contour[2] = Point(2.0f, 0.0f);
    contour[3] = Point(1.0f, -1.0f);
    contour[4] = Point(0.0f, 0.0f);

    auto [dx4, dy4] = contour.fourthDiff(2);
    //  ([index-2] - 4.0f * [index-1].X + 6.0f * [index].X - 4.0f * [index+1].X + [index+2].X);
    EXPECT_FLOAT_EQ(dx4, 4.0f); // 0 - 4 + 12 -4 + 0 = 4
    EXPECT_FLOAT_EQ(dy4, 0.0f); // 0 -4 + 0 +4 + 0 = 0
}

TEST(ELSnakeEngineTest, operatorOverloadTest) {
    Contour contour(5, Point(0,0), 5);
    
    contour[0] = Point(0.0f, 0.0f);
    contour[1] = Point(1.0f, 1.0f);
    contour[2] = Point(2.0f, 0.0f);
    contour[3] = Point(1.0f, -1.0f);
    contour[4] = Point(0.0f, 0.0f);

    Point p1 = contour[7];
    EXPECT_FLOAT_EQ(p1.X, 2.0f);
    EXPECT_FLOAT_EQ(p1.Y, 0.0f);

    Point p2 = contour[-1]; 
    EXPECT_FLOAT_EQ(p2.X, 0.0f);
    EXPECT_FLOAT_EQ(p2.Y, 0.0f);
}