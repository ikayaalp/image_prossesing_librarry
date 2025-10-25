#pragma once

#include "Filter.h"

namespace ImageProcessing {

/**
 * @brief Brightness adjustment filter
 * Demonstrates inheritance and polymorphism
 */
class BrightnessAdjust : public Filter {
public:
    explicit BrightnessAdjust(double brightness_factor = 1.0);
    
    // Filter interface implementation
    std::unique_ptr<Image> apply(const Image& input) const override;
    std::string getName() const override { return "BrightnessAdjust"; }
    void setParameters(const std::map<std::string, double>& params) override;
    std::map<std::string, double> getParameters() const override;
    std::unique_ptr<Filter> clone() const override;
    
    // Specific methods
    void setBrightnessFactor(double factor);
    double getBrightnessFactor() const { return brightness_factor_; }

private:
    double brightness_factor_;
};

} // namespace ImageProcessing
