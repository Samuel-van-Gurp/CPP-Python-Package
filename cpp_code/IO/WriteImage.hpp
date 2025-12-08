#ifndef WRITE_IMAGE_HPP
#define WRITE_IMAGE_HPP
#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "DataObjects/Contour.hpp"
#include <iostream>
#include <fstream>

class WriteImage
{
    public:
        WriteImage();
        void saveImage(const ImageHolder<float> &image);

        void ContourOverLay(const Contour &contour, const ImageHolder<float> &image, const ImageProcessorFacade &processor);

        
        private:
        void SaveAsPGM(const ImageHolder<float> &image);
        // void SaveAsPGM(const std::vector<std::vector<uint8_t>> &image);
};

#endif // WRITE_IMAGE_HPP