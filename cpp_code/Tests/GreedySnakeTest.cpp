#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "DataObjects/Point.hpp"
#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "ImageProcessing/IConvolve.hpp"
#include "ImageProcessing/IIntensityManipulator.hpp" 
#include "ImageProcessing/NaiveConvolve.hpp"
#include "ImageProcessing/IntensityManipulator.hpp"
#include "Test_Classes/GreedySnakeTestWrapper.cpp"

TEST(GreedySnakeEngineTest, constructInternalEnergyMatrix) {
    
    // Arrange
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
    ImageProcessorFacade imageProcessor(std::make_unique<NaiveConvolve>(), std::make_unique<IntensityManipulator>());

    GreedySnakeTestWrapper SUT = GreedySnakeTestWrapper(imageProcessor, imageHolder, contour, 0.5f, 0.5f);
    
    // Act
    auto InternalEnergyMatrix = SUT.constructInternalEnergyMatrix(0, Point(2,2));

    // Assert
    EXPECT_NEAR(InternalEnergyMatrix[0][0], 1.000000f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[0][1], 0.381966f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[0][2], 0.527864f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[1][0], 0.618034f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[1][1], 0.000000f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[1][2], 0.145898f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[2][0], 1.000000f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[2][1], 0.381966f, 1e-5f);
    EXPECT_NEAR(InternalEnergyMatrix[2][2], 0.527864f, 1e-5f);
}

TEST(GreedySnakeEngineTest, constructExternalEnergyMatrix) {
    
    // Arrange
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
    ImageProcessorFacade imageProcessor(std::make_unique<NaiveConvolve>(), std::make_unique<IntensityManipulator>());

    GreedySnakeTestWrapper SUT = GreedySnakeTestWrapper(imageProcessor, imageHolder, contour, 0.5f, 0.5f);
    
    Point testPoint(2,2);

    // Act
    auto ExternalEnergyMatrix = SUT.constructExternalEnergyMatrix(testPoint);

    // Assert
    EXPECT_NEAR(ExternalEnergyMatrix[0][0], 0.312283f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[0][1], 0.25196f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[0][2], 0.806815f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[1][0], 0.0595293f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[1][1], 0.0f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[1][2], 0.600393f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[2][0], 0.559381f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[2][1], 0.509912f, 1e-5f);
    EXPECT_NEAR(ExternalEnergyMatrix[2][2], 1.0f, 1e-5f);
    } 

TEST(GreedySnakeEngineTest, combineEnergyMatrix) {
        // Arrange
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
    ImageProcessorFacade imageProcessor(std::make_unique<NaiveConvolve>(), std::make_unique<IntensityManipulator>());

    GreedySnakeTestWrapper SUT = GreedySnakeTestWrapper(imageProcessor, imageHolder, contour, 0.5f, 0.5f);

    std::vector<std::vector<float>> InternalEnergyMatrix = {
        {0.1f, 0.2f, 0.3f},
        {0.4f, 0.5f, 0.6f},
        {0.7f, 0.8f, 0.9f}
    };

    std::vector<std::vector<float>> ExternalEnergyMatrix = {
        {0.9f, 0.8f, 0.7f},
        {0.6f, 0.5f, 0.4f},
        {0.3f, 0.2f, 0.1f}
    };

    auto cominedMatrix = SUT.combineEnergyMatrix(InternalEnergyMatrix, ExternalEnergyMatrix, 1.0f, 1.0f);

    // Assert
    EXPECT_NEAR(cominedMatrix[0][0], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[0][1], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[0][2], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[1][0], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[1][1], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[1][2], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[2][0], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[2][1], 1.0f, 1e-5f);
    EXPECT_NEAR(cominedMatrix[2][2], 1.0f, 1e-5f);
}

TEST(GreedySnakeEngineTest, normalizeEnergyMatrix_Test) {
    // Arrange

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
    ImageProcessorFacade imageProcessor(std::make_unique<NaiveConvolve>(), std::make_unique<IntensityManipulator>());

    GreedySnakeTestWrapper SUT = GreedySnakeTestWrapper(imageProcessor, imageHolder, contour, 0.5f, 0.5f);

    // Act
    auto normalizedMatrix = SUT.normalizeEnergyMatrix<float>({{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}});

    // Assert
    EXPECT_NEAR(normalizedMatrix[0][0], 0.0f,   1e-5f);
    EXPECT_NEAR(normalizedMatrix[0][1], 0.125f, 1e-5f);
    EXPECT_NEAR(normalizedMatrix[0][2], 0.25f,  1e-5f);
    EXPECT_NEAR(normalizedMatrix[1][0], 0.375f, 1e-5f);
    EXPECT_NEAR(normalizedMatrix[1][1], 0.5f,   1e-5f);
    EXPECT_NEAR(normalizedMatrix[1][2], 0.625f, 1e-5f);
    EXPECT_NEAR(normalizedMatrix[2][0], 0.75f,  1e-5f);
    EXPECT_NEAR(normalizedMatrix[2][1], 0.875f, 1e-5f);
    EXPECT_NEAR(normalizedMatrix[2][2], 1.0f,   1e-5f);   
}

TEST(GreedySnakeEngineTest, geNextStep){
    // Arrange
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
    ImageProcessorFacade imageProcessor(std::make_unique<NaiveConvolve>(), std::make_unique<IntensityManipulator>());

    GreedySnakeTestWrapper SUT = GreedySnakeTestWrapper(imageProcessor, imageHolder, contour, 0.5f, 0.5f);

    Point testPoint(2,2);

    // Act
    auto nextPoint = SUT.getNextStep(0, testPoint, contour);

    // Assert
    EXPECT_NEAR(nextPoint.X, 2.0f, 1e-5f);
    EXPECT_NEAR(nextPoint.Y, 2.0f, 1e-5f);
}

