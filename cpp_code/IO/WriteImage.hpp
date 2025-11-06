#ifndef WRITE_IMAGE_HPP
#define WRITE_IMAGE_HPP
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Contour.hpp"
#include <iostream>
#include <fstream>

class WriteImage
{
    public:
        WriteImage();
        void saveImage(const ImageHolder<uint8_t> &image);

        void ContourOverLay(const Contour &contour, const ImageHolder<uint8_t> &image, const ImageProcessor &processor);

        
        private:
        void SaveAsPGM(const ImageHolder<uint8_t> &image);
        // void SaveAsPGM(const std::vector<std::vector<uint8_t>> &image);
};

#endif // WRITE_IMAGE_HPP