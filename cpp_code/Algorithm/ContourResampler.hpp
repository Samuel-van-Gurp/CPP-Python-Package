#ifndef CONTOURRESAMPLER_HPP
#define CONTOURRESAMPLER_HPP

#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include <vector>
#include <algorithm>

class ContourResampler {

    public:
        static Contour resampleContour(const Contour &contour);
    
    private:
        static Point interpolate(const Point &p1, const Point &p2, double d1, double d2, double targetD);

        static std::vector<float> constructTargetSpacingVector(float targetSpacing, int length);

        static float computeTargetSpacing(const Contour &contour);
        static std::vector<float> constructSegmentLengthVector(const Contour &contour);
        static std::vector<float> constructAccumulatedDistanceVector(const std::vector<float> &segmentLengths);
        static float distanceBetweenPoints(const Point &p1, const Point &p2);
};

#endif // CONTOURRESAMPLER_HPP
