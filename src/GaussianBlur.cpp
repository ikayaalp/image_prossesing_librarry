#include "GaussianBlur.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace ImageProcessing {

// Gaussian blur filtresi oluşturur (sigma ve kernel boyutu ile)
GaussianBlur::GaussianBlur(double sigma, int kernel_size)
    : sigma_(sigma), kernel_size_(kernel_size) {
    if (sigma <= 0 || kernel_size <= 0 || kernel_size % 2 == 0) {
        throw std::invalid_argument("Sigma must be positive, kernel size must be positive odd number");
    }
    generateKernel();
}

// Görüntüye Gaussian blur filtresini uygular
std::unique_ptr<Image> GaussianBlur::apply(const Image& input) const {
    auto output = std::make_unique<Image>(input.getWidth(), input.getHeight(), input.getChannels());
    
    int half_kernel = kernel_size_ / 2;
    
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            for (int channel = 0; channel < input.getChannels(); ++channel) {
                double sum = 0.0;
                double weight_sum = 0.0;
                
                for (int ky = -half_kernel; ky <= half_kernel; ++ky) {
                    for (int kx = -half_kernel; kx <= half_kernel; ++kx) {
                        int nx = x + kx;
                        int ny = y + ky;
                        
                        if (nx >= 0 && nx < input.getWidth() && 
                            ny >= 0 && ny < input.getHeight()) {
                            
                            double weight = kernel_[ky + half_kernel][kx + half_kernel];
                            sum += input.at(nx, ny, channel) * weight;
                            weight_sum += weight;
                        }
                    }
                }
                
                if (weight_sum > 0) {
                    output->at(x, y, channel) = static_cast<Image::Pixel>(
                        std::round(sum / weight_sum));
                }
            }
        }
    }
    
    return output;
}

// Filtre parametrelerini ayarlar (map ile)
void GaussianBlur::setParameters(const std::map<std::string, double>& params) {
    auto sigma_it = params.find("sigma");
    if (sigma_it != params.end()) {
        setSigma(sigma_it->second);
    }
    
    auto size_it = params.find("kernel_size");
    if (size_it != params.end()) {
        setKernelSize(static_cast<int>(size_it->second));
    }
}

// Mevcut filtre parametrelerini döndürür
std::map<std::string, double> GaussianBlur::getParameters() const {
    return {{"sigma", sigma_}, {"kernel_size", static_cast<double>(kernel_size_)}};
}

// Filtrenin bir kopyasını oluşturur (clone pattern)
std::unique_ptr<Filter> GaussianBlur::clone() const {
    return std::make_unique<GaussianBlur>(*this);
}

// Sigma değerini ayarlar ve kernel'i yeniden oluşturur
void GaussianBlur::setSigma(double sigma) {
    if (sigma <= 0) {
        throw std::invalid_argument("Sigma must be positive");
    }
    sigma_ = sigma;
    generateKernel();
}

// Kernel boyutunu ayarlar ve kernel'i yeniden oluşturur
void GaussianBlur::setKernelSize(int size) {
    if (size <= 0 || size % 2 == 0) {
        throw std::invalid_argument("Kernel size must be positive odd number");
    }
    kernel_size_ = size;
    generateKernel();
}

// Gaussian kernel'ini oluşturur ve normalize eder
void GaussianBlur::generateKernel() {
    kernel_.assign(kernel_size_, std::vector<double>(kernel_size_, 0.0));
    
    int half_kernel = kernel_size_ / 2;
    double sum = 0.0;
    
    for (int y = 0; y < kernel_size_; ++y) {
        for (int x = 0; x < kernel_size_; ++x) {
            double dx = x - half_kernel;
            double dy = y - half_kernel;
            kernel_[y][x] = gaussianFunction(dx, dy);
            sum += kernel_[y][x];
        }
    }
    
    // Normalize kernel
    for (auto& row : kernel_) {
        for (auto& value : row) {
            value /= sum;
        }
    }
}

// Gaussian fonksiyonunu hesaplar (2D Gaussian dağılımı)
double GaussianBlur::gaussianFunction(double x, double y) const {
    double exponent = -(x * x + y * y) / (2.0 * sigma_ * sigma_);
    return std::exp(exponent) / (2.0 * M_PI * sigma_ * sigma_);
}

} // namespace ImageProcessing
