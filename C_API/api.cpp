#include <stdio.h>
#include "cpp_code/api_impl.hpp"
#include "api.h"


#ifdef __cplusplus
extern "C" {
#endif

EXPORT_API void PrepImageAndSnake(ImageInfo const* imageInfo, SnakeParams const* params) 
{

    ImageHolder<float>* imageHolder_ptr = constructImage(imageInfo->data, 
                                                         imageInfo->width, 
                                                         imageInfo->height, 
                                                         imageInfo->stride);
    
    SnakeController* snakeInterface_ptr = setupSnake(imageHolder_ptr, params->alpha, 
                                                                     params->beta, 
                                                                     params->iterations, 
                                                                     params->contour_center_x, 
                                                                     params->contour_center_y, 
                                                                     params->contour_radius, 
                                                                     params->contour_points);

    snakeInterface_ptr->run(params->iterations);                        
                                                
    delete imageHolder_ptr;
    delete snakeInterface_ptr;
}

#ifdef __cplusplus
}
#endif