#include <stdio.h>
#include "api.hpp"


std::vector<Point> runSnake(ImageInfo const *imageInfo, SnakeParams params)
{
    ImageHolder<float>* imageHolder_ptr = new ImageHolder<float>(imageInfo->data,
                                                                 imageInfo->width,
                                                                 imageInfo->height);

    SnakeController* snakeInterface_ptr = setupSnake(imageHolder_ptr, params.alpha,
                                                                     params.beta,
                                                                     params.iterations,
                                                                     params.contour_center_x,
                                                                     params.contour_center_y,
                                                                     params.contour_radius_x,
                                                                     params.contour_radius_y,
                                                                     params.contour_points);

    return snakeInterface_ptr->run(params.iterations);
}

