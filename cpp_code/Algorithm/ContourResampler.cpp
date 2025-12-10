#include "ContourResampler.hpp"


Contour ContourResampler::resampleContour(const Contour &contour) {
    
    float targetSpacing = computeTargetSpacing(contour);
    auto targetSpacingVector = constructTargetSpacingVector(targetSpacing, contour.Size());
    auto SegmentLengthVector = constructSegmentLengthVector(contour);
    auto AccumulatedDistanceVector = constructAccumulatedDistanceVector(SegmentLengthVector);

    float totalLength = AccumulatedDistanceVector.back();

    Contour newContour = contour;

    for (int i = 0; i < contour.Size(); ++i) 
    {
        float currentTargetDistance = targetSpacingVector[i];

        auto it_upper = std::upper_bound(AccumulatedDistanceVector.begin(), 
                                    AccumulatedDistanceVector.end(), 
                                    currentTargetDistance);

        size_t j = std::max(0, (int)std::distance(AccumulatedDistanceVector.begin(), it_upper) - 1);

        const Point& p1 = contour[j];
        const Point& p2 = contour[j + 1];      
        double d1 = AccumulatedDistanceVector[j];    
        double d2 = AccumulatedDistanceVector[j + 1]; 
       
        Point newPoint = interpolate(p1, p2, d1, d2, currentTargetDistance);
        
        newContour[i] = newPoint;
    }


    return newContour;
}

Point ContourResampler::interpolate(const Point& p1, const Point& p2, double d1, double d2, double targetD) {
  
    double segmentLength = d2 - d1;
    if (std::abs(segmentLength) < 1e-9) {
        return p1;
    }
    
    double t = (targetD - d1) / segmentLength; 

    float new_x = p1.X + static_cast<float>(t * (p2.X - p1.X));
    float new_y = p1.Y + static_cast<float>(t * (p2.Y - p1.Y));
    
    return {new_x, new_y};
}

std::vector<float> ContourResampler::constructTargetSpacingVector(float targetSpacing, int length)
{
    std::vector<float> spacingVector(length);
    for (int i = 0; i < length; ++i) {
        spacingVector[i] = targetSpacing * i;
    }
    return spacingVector;
}

float ContourResampler::computeTargetSpacing(const Contour &contour)
{
    int N = contour.Size();
    float dist = 0.0f;
    for (int i = 0; i < N; ++i)
    {
        Point p1 = contour[i];
        Point p2 = contour[(i + 1) % N];
        dist += distanceBetweenPoints(p1, p2);
    }
    return dist / static_cast<float>(N);
}

std::vector<float> ContourResampler::constructSegmentLengthVector(const Contour &contour)
{
    int N = contour.Size();
    std::vector<float> distances;
    distances.reserve(N);
    for (int i = 0; i < N; ++i)
    {
        distances.push_back(distanceBetweenPoints(contour[i], contour[(i + 1) % N]));
    }
    return distances;
}

std::vector<float> ContourResampler::constructAccumulatedDistanceVector(const std::vector<float> &segmentLengths)
{
    std::vector<float> accumulatedDistances;
    accumulatedDistances.reserve(segmentLengths.size() + 1);
    float total = 0.0f;
    accumulatedDistances.push_back(total); // add zero
    for (const auto &length : segmentLengths)
    {
        total += length;
        accumulatedDistances.push_back(total);
    }
    return accumulatedDistances; 
}

float ContourResampler::distanceBetweenPoints(const Point &p1, const Point &p2)
{
    float dx = p2.X - p1.X;
    float dy = p2.Y - p1.Y;
    return std::sqrt(dx * dx + dy * dy);
}
