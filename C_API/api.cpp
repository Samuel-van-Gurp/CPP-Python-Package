#include <stdio.h>
#include "cpp_code/api_impl.hpp"
#include "api.h"


#ifdef __cplusplus
extern "C" {
#endif

EXPORT_API void runSnake(ImageInfo const* imageInfo, SnakeParams const* params, Point* out_contour, int* out_count_ptr) 
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

     
    if (out_count_ptr) 
    {
        Point* contour_ptr = snakeInterface_ptr->run(params->iterations, out_count_ptr);            
        std::copy(contour_ptr, 
                contour_ptr + (*out_count_ptr), 
                out_contour);
    }   
    



    delete imageHolder_ptr;
    delete snakeInterface_ptr;

}


#ifdef __cplusplus
}
#endif

