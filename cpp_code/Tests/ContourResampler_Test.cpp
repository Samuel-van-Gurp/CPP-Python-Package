#include <gtest/gtest.h>
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "Algorithm/ContourResampler.hpp"

TEST(ContourResamplerTest, testdoesnothingForAlreadyEvenlySpacedContour) {
    Contour contour(2, Point(0,0), 4); // set up a circle contour

    // resampling this contour should yield the same points
    Contour resampledContour = ContourResampler::resampleContour(contour);
    
    EXPECT_NEAR(resampledContour[0].X,  2.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[0].Y,  0.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[1].X,  0.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[1].Y,  2.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[2].X, -2.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[2].Y,  0.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[3].X,  0.0f, 1e-5f);
    EXPECT_NEAR(resampledContour[3].Y, -2.0f, 1e-5f);
}

