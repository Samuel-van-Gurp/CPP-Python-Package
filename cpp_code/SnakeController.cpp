#include "SnakeController.hpp"
#include "Algorithm/GreedySnakeEngine.hpp"
#include "Algorithm/ELSnakeEngine.hpp"
#include <memory>

SnakeController::SnakeController(ImageHolder<float> imageHolder, std::unique_ptr<ImageProcessorFacade> imageProcessor, 
                                                                 Contour contour, 
                                                                 std::unique_ptr<ISnakeEngine> engine)
    : m_imageHolder(imageHolder)
    , m_imageProcessor(std::move(imageProcessor))
    , m_contour(contour)
    , m_engine(std::move(engine))
{
}

std::vector<Point> SnakeController::run(int iterations)
{
    return m_engine->RunSnake(iterations, m_contour);
}

SnakeController SnakeController::createSnakeController(ImageHolder<float> imageHolder, SnakeParams snakeParams, enum SnakeSolver solver)
{
    // make ImageProcessorFacade and inject helper classes
    auto imageProcessor = std::make_unique<ImageProcessorFacade>(std::make_unique<NaiveConvolve>(), std::make_unique<IntensityManipulator>());

    // make initial contour (TODO: make a factory method that only takes SnakeParams)
    Contour contour(snakeParams.contour_radius_x, snakeParams.contour_radius_y, Point(snakeParams.contour_center_x, snakeParams.contour_center_y), snakeParams.contour_points);

    std::unique_ptr<ISnakeEngine> engine;

    switch(solver) {
        case SnakeSolver::GREEDY_ALGORITHM:
            engine = std::make_unique<GreedySnakeEngine>(*imageProcessor, imageHolder, contour, snakeParams.alpha, snakeParams.beta);
            break;
        case SnakeSolver::EULER_LAGRANGE:
            {
            auto gradients = imageProcessor->PrepareImageForELSnake(imageHolder);
            engine = std::make_unique<ELSnakeEngine>(gradients, snakeParams.alpha, snakeParams.beta);
            }
            break;
        default:
            engine = std::make_unique<GreedySnakeEngine>(*imageProcessor, imageHolder, contour, snakeParams.alpha, snakeParams.beta);
            break;
    }

    return SnakeController(imageHolder, std::move(imageProcessor), contour, std::move(engine));
}

