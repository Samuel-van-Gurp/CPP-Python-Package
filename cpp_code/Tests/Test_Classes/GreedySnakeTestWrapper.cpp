#include "Algorithm/GreedySnakeEngine.hpp"

class GreedySnakeTestWrapper : public GreedySnakeEngine {
public:
    using GreedySnakeEngine::GreedySnakeEngine;

    using GreedySnakeEngine::normalizeEnergyMatrix;
    using GreedySnakeEngine::combineEnergyMatrix;
    using GreedySnakeEngine::constructInternalEnergyMatrix;
    using GreedySnakeEngine::constructExternalEnergyMatrix;
    using GreedySnakeEngine::getNextStep;

    using GreedySnakeEngine::m_imageProcessor;
    using GreedySnakeEngine::m_imageHolder; 
    using GreedySnakeEngine::m_contour;
    using GreedySnakeEngine::alpha;
    using GreedySnakeEngine::beta;

};