#include "EdgeDetection.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace GorselIsleme {

EdgeDetection::EdgeDetection(Direction direction) : direction_(direction) {
    initializeSobelKernels();
}

std::unique_ptr<Image> EdgeDetection::apply(const Image& input) const {
    auto output = std::make_unique<Image>(input.getWidth(), input.getHeight(), input.getChannels());
    
    for (int y = 1; y < input.getHeight() - 1; ++y) {
        for (int x = 1; x < input.getWidth() - 1; ++x) {
            for (int channel = 0; channel < input.getChannels(); ++channel) {
                double gx = 0.0, gy = 0.0;
                
                if (direction_ == Horizontal || direction_ == Both) {
                    gx = applyConvolution(input, x, y, sobel_x_);
                }
                
                if (direction_ == Vertical || direction_ == Both) {
                    gy = applyConvolution(input, x, y, sobel_y_);
                }
                
                double magnitude = std::sqrt(gx * gx + gy * gy);
                output->at(x, y, channel) = static_cast<Image::Pixel>(
                    std::min(255.0, std::max(0.0, magnitude)));
            }
        }
    }
    
    return output;
}

std::unique_ptr<Filter> EdgeDetection::clone() const {
    return std::make_unique<EdgeDetection>(*this);
}

void EdgeDetection::setDirection(Direction direction) {
    direction_ = direction;
}

void EdgeDetection::initializeSobelKernels() {
    sobel_x_ = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    sobel_y_ = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
}

double EdgeDetection::applyConvolution(const Image& image, int x, int y, 
                                     const std::vector<std::vector<int>>& kernel) const {
    double sum = 0.0;
    
    for (int ky = 0; ky < 3; ++ky) {
        for (int kx = 0; kx < 3; ++kx) {
            int nx = x + kx - 1;
            int ny = y + ky - 1;
            
            if (nx >= 0 && nx < image.getWidth() && 
                ny >= 0 && ny < image.getHeight()) {
                sum += image.at(nx, ny) * kernel[ky][kx];
            }
        }
    }
    
    return sum;
}

} // namespace GorselIsleme