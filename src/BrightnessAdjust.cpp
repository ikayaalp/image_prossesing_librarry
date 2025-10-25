#include "BrightnessAdjust.h"
#include <algorithm>
#include <cmath>

namespace ImageProcessing {

// Parlaklık ayarlama filtresi oluşturur (parlaklık çarpanı ile)
BrightnessAdjust::BrightnessAdjust(double brightness_factor)
    : brightness_factor_(brightness_factor) {
    if (brightness_factor < 0) {
        throw std::invalid_argument("Brightness factor must be non-negative");
    }
}

// Görüntüye parlaklık ayarlama filtresini uygular
std::unique_ptr<Image> BrightnessAdjust::apply(const Image& input) const {
    auto output = std::make_unique<Image>(input.getWidth(), input.getHeight(), input.getChannels());
    
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            for (int channel = 0; channel < input.getChannels(); ++channel) {
                double new_value = input.at(x, y, channel) * brightness_factor_;
                output->at(x, y, channel) = static_cast<Image::Pixel>(
                    std::min(255.0, std::max(0.0, std::round(new_value))));
            }
        }
    }
    
    return output;
}

// Filtre parametrelerini ayarlar (parlaklık çarpanı ile)
void BrightnessAdjust::setParameters(const std::map<std::string, double>& params) {
    auto factor_it = params.find("brightness_factor");
    if (factor_it != params.end()) {
        setBrightnessFactor(factor_it->second);
    }
}

// Mevcut filtre parametrelerini döndürür
std::map<std::string, double> BrightnessAdjust::getParameters() const {
    return {{"brightness_factor", brightness_factor_}};
}

// Filtrenin bir kopyasını oluşturur (clone pattern)
std::unique_ptr<Filter> BrightnessAdjust::clone() const {
    return std::make_unique<BrightnessAdjust>(*this);
}

// Parlaklık çarpanını ayarlar
void BrightnessAdjust::setBrightnessFactor(double factor) {
    if (factor < 0) {
        throw std::invalid_argument("Brightness factor must be non-negative");
    }
    brightness_factor_ = factor;
}

} // namespace ImageProcessing
