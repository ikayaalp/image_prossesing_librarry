#pragma once
#include "Filter.h"

namespace GorselIsleme {

class BrightnessAdjust : public Filter {
public:
    BrightnessAdjust(double brightness_factor = 1.0);
    
    std::unique_ptr<Image> apply(const Image& input) const override;
    std::string getName() const override { return "BrightnessAdjust"; }
    std::unique_ptr<Filter> clone() const override;
    
    void setBrightnessFactor(double factor);
    
private:
    double brightness_factor_;
};

} // namespace GorselIsleme