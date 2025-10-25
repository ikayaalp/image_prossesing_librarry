#include "EdgeDetection.h"
#include <cmath>
#include <algorithm>

namespace ImageProcessing {

// Kenar tespiti filtresi oluşturur (yatay, dikey veya her ikisi)
EdgeDetection::EdgeDetection(Direction direction) : direction_(direction) {
    initializeSobelKernels();
}

// Görüntüye kenar tespiti filtresini uygular (Sobel operatörü)
std::unique_ptr<Image> EdgeDetection::apply(const Image& input) const {
    auto output = std::make_unique<Image>(input.getWidth(), input.getHeight(), input.getChannels());
    
    for (int y = 1; y < input.getHeight() - 1; ++y) {
        for (int x = 1; x < input.getWidth() - 1; ++x) {
            for (int channel = 0; channel < input.getChannels(); ++channel) {
                double gx = 0.0, gy = 0.0;
                
                if (direction_ == Direction::Horizontal || direction_ == Direction::Both) {
                    gx = applyConvolution(input, x, y, sobel_x_);
                }
                
                if (direction_ == Direction::Vertical || direction_ == Direction::Both) {
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

// Filtre parametrelerini ayarlar (yön parametresi ile)
void EdgeDetection::setParameters(const std::map<std::string, double>& params) {
    auto direction_it = params.find("direction");
    if (direction_it != params.end()) {
        setDirection(static_cast<Direction>(static_cast<int>(direction_it->second)));
    }
}

// Mevcut filtre parametrelerini döndürür
std::map<std::string, double> EdgeDetection::getParameters() const {
    return {{"direction", static_cast<double>(static_cast<int>(direction_))}};
}

// Filtrenin bir kopyasını oluşturur (clone pattern)
std::unique_ptr<Filter> EdgeDetection::clone() const {
    return std::make_unique<EdgeDetection>(*this);
}

// Kenar tespiti yönünü ayarlar (yatay, dikey veya her ikisi)
void EdgeDetection::setDirection(Direction direction) {
    direction_ = direction;
}

// Sobel kernel'lerini başlatır (yatay ve dikey kenar tespiti için)
void EdgeDetection::initializeSobelKernels() {
    sobel_x_ = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    sobel_y_ = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
}

// Konvolüsyon işlemini uygular (kernel ile piksel çarpımı)
double EdgeDetection::applyConvolution(const Image& image, int x, int y, 
                                     const std::vector<std::vector<int>>& kernel) const {
    double sum = 0.0;
    
    for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
            int nx = x + kx;
            int ny = y + ky;
            if (nx >= 0 && nx < image.getWidth() && 
                ny >= 0 && ny < image.getHeight()) {
                sum += image.at(nx, ny) * kernel[ky + 1][kx + 1];
            }
        }
    }
    
    return sum;
}

} // namespace ImageProcessing
