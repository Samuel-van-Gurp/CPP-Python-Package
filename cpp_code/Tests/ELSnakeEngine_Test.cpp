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
    // Create mock or stub implementations of IConvolver and IIntensityManipulator
    auto convolver = std::make_unique<NaiveConvolve>();
    auto intensityManipulator = std::make_unique<IntensityManipulator>();
    ImageProcessorFacade imageProcessor(std::move(convolver), std::move(intensityManipulator));

    ELSnakeEngine snakeEngine(imageProcessor, imageHolder, contour, 0.5f, 0.5f);

    std::tuple<float, float> internalForce = std::make_tuple(2.0f, 4.0f);
    std::tuple<float, float> externalForce = std::make_tuple(6.0f, 8.0f);

    auto combinedForce = snakeEngine.combineForces(internalForce, externalForce);

    EXPECT_FLOAT_EQ(std::get<0>(combinedForce), 4.0f);
    EXPECT_FLOAT_EQ(std::get<1>(combinedForce), 6.0f); 
}


