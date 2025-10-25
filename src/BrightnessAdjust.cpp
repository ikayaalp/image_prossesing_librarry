#include "BrightnessAdjust.h"
#include <algorithm>
#include <stdexcept>

namespace GorselIsleme {

BrightnessAdjust::BrightnessAdjust(double brightness_factor)
    : brightness_factor_(brightness_factor) {
    if (brightness_factor < 0) {
        throw std::invalid_argument("Parlaklik faktoru negatif olamaz");
    }
}

std::unique_ptr<Image> BrightnessAdjust::apply(const Image& input) const {
    auto output = std::make_unique<Image>(input.getWidth(), input.getHeight(), input.getChannels());
    
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            for (int channel = 0; channel < input.getChannels(); ++channel) {
                double new_value = input.at(x, y, channel) * brightness_factor_;
                output->at(x, y, channel) = static_cast<Image::Pixel>(
                    std::min(255.0, std::max(0.0, new_value)));
            }
        }
    }
    
    return output;
}

std::unique_ptr<Filter> BrightnessAdjust::clone() const {
    return std::make_unique<BrightnessAdjust>(*this);
}

void BrightnessAdjust::setBrightnessFactor(double factor) {
    if (factor < 0) {
        throw std::invalid_argument("Parlaklik faktoru negatif olamaz");
    }
    brightness_factor_ = factor;
}

} // namespace GorselIsleme