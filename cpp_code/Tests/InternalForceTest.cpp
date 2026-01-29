#include <gtest/gtest.h>
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "Algorithm/ContourResampler.hpp"

TEST(internalForceTest, testforce) {
    Contour contour(100); // init empty contour

    // this is a elips shape
    // Rightmost (max X): index 0
    // Leftmost (min X): index 50
    // Topmost (max Y): index 25
    // Bottommost(min Y): index 75
    // semi major axis: 240
    // semi minor axis: 6

    std::vector<Point> pts = {
        {263.5645f, 66.77419f}, {263.32773f, 67.15093f}, {262.6183f, 67.52619f},
        {261.439f, 67.89848f}, {259.7945f, 68.266335f}, {257.69128f, 68.628296f},
        {255.1377f, 68.98294f}, {252.14375f, 69.328865f}, {248.72131f, 69.66472f},
        {244.88387f, 69.98915f}, {240.64656f, 70.3009f}, {236.02611f, 70.59874f},
        {231.04076f, 70.88148f}, {225.71016f, 71.148f}, {220.05539f, 71.39727f},
        {214.09874f, 71.628296f}, {207.86372f, 71.84016f}, {201.37495f, 72.032036f},
        {194.65804f, 72.203156f}, {187.73946f, 72.35285f}, {180.64656f, 72.48053f},
        {173.4073f, 72.58569f}, {166.05028f, 72.667915f}, {158.6045f, 72.72688f},
        {151.09938f, 72.76235f}, {143.56451f, 72.77419f}, {136.02965f, 72.76235f},
        {128.52452f, 72.72688f}, {121.07876f, 72.667915f}, {113.721725f, 72.58569f},
        {106.482475f, 72.48053f}, {99.389565f, 72.35285f}, {92.471f, 72.203156f},
        {85.754074f, 72.032036f}, {79.2653f, 71.84016f}, {73.03028f, 71.628296f},
        {67.07364f, 71.39727f}, {61.41886f, 71.148f}, {56.08828f, 70.88148f},
        {51.102924f, 70.59874f}, {46.482475f, 70.3009f}, {42.245163f, 69.98915f},
        {38.40771f, 69.66472f}, {34.985268f, 69.328865f}, {31.991335f, 68.98294f},
        {29.437733f, 68.628296f}, {27.334536f, 68.266335f}, {25.690044f, 67.89848f},
        {24.51075f, 67.52619f}, {23.801308f, 67.15093f}, {23.564514f, 66.77419f},
        {23.801308f, 66.39745f}, {24.51075f, 66.022194f}, {25.690044f, 65.6499f},
        {27.334536f, 65.28205f}, {29.437733f, 64.92009f}, {31.991335f, 64.565445f},
        {34.985268f, 64.21952f}, {38.40771f, 63.88367f}, {42.245163f, 63.55923f},
        {46.482475f, 63.247482f}, {51.102924f, 62.94965f}, {56.08828f, 62.666912f},
        {61.41886f, 62.40038f}, {67.07364f, 62.15111f}, {73.03028f, 61.92009f},
        {79.2653f, 61.708225f}, {85.754074f, 61.516354f}, {92.471f, 61.34523f},
        {99.389565f, 61.195534f}, {106.482475f, 61.067852f}, {113.721725f, 60.962692f},
        {121.07876f, 60.88047f}, {128.52452f, 60.821507f}, {136.02965f, 60.786034f},
        {143.56451f, 60.774193f}, {151.09938f, 60.786034f}, {158.6045f, 60.821507f},
        {166.05028f, 60.88047f}, {173.4073f, 60.962692f}, {180.64656f, 61.067852f},
        {187.73946f, 61.195534f}, {194.65804f, 61.34523f}, {201.37495f, 61.516354f},
        {207.86372f, 61.708225f}, {214.09874f, 61.92009f}, {220.05539f, 62.15111f},
        {225.71016f, 62.40038f}, {231.04076f, 62.666912f}, {236.02611f, 62.94965f},
        {240.64656f, 63.247482f}, {244.88387f, 63.55923f}, {248.72131f, 63.88367f},
        {252.14375f, 64.21952f}, {255.1377f, 64.565445f}, {257.69128f, 64.92009f},
        {259.7945f, 65.28205f}, {261.439f, 65.6499f}, {262.6183f, 66.022194f},
        {263.32773f, 66.39745f}
    };

    
    // copy into contour
    for (int i = 0; i < contour.Size(); ++i) {
        contour[i] = pts[static_cast<std::size_t>(i)];

        if (i == 0 || i == 25 || i == 50 || i == 75) {
            std::cerr << "Point " << i << ": (" << contour[i].X << ", " << contour[i].Y << ")\n";
            std::cerr.flush();
        }
    }


    // print the second diff at index 0,25,50,75
    auto secondDiff0 = contour.secondDiff(0);
    auto secondDiff25 = contour.secondDiff(25);
    auto secondDiff50 = contour.secondDiff(50);
    auto secondDiff75 = contour.secondDiff(75);

    EXPECT_FLOAT_EQ(std::get<0>(secondDiff0), -2.3916016f);
    EXPECT_FLOAT_EQ(std::get<1>(secondDiff0), -3.0517578e-05f);
    EXPECT_FLOAT_EQ(std::get<0>(secondDiff25), 0.0f);
    EXPECT_FLOAT_EQ(std::get<1>(secondDiff25), -0.00041723251f);
    EXPECT_FLOAT_EQ(std::get<0>(secondDiff50), 2.3917999f);
    EXPECT_FLOAT_EQ(std::get<1>(secondDiff50), 0.0f);
    EXPECT_FLOAT_EQ(std::get<0>(secondDiff75), 0.0f);
    EXPECT_FLOAT_EQ(std::get<1>(secondDiff75), 0.00041723251f);  

    // print the fourth diff at index 0,25,50,75
    auto fourthDiff0 = contour.fourthDiff(0);
    auto fourthDiff25 = contour.fourthDiff(25); 
    auto fourthDiff50 = contour.fourthDiff(50);
    auto fourthDiff75 = contour.fourthDiff(75);

    EXPECT_FLOAT_EQ(std::get<0>(fourthDiff0), 0.046705723f);
    EXPECT_FLOAT_EQ(std::get<1>(fourthDiff0), -0.0005838215f);
    EXPECT_FLOAT_EQ(std::get<0>(fourthDiff25), -4.7338631e-09f);
    EXPECT_FLOAT_EQ(std::get<1>(fourthDiff25), 2.8403178e-08f);
    EXPECT_FLOAT_EQ(std::get<0>(fourthDiff50), -0.047871608f);
    EXPECT_FLOAT_EQ(std::get<1>(fourthDiff50), -0.00058380014f);
    EXPECT_FLOAT_EQ(std::get<0>(fourthDiff75), 4.7338631e-09f);
    EXPECT_FLOAT_EQ(std::get<1>(fourthDiff75), -3.6687439e-08f);
}