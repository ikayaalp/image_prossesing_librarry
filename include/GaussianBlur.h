#pragma once

#include "Filter.h"
#include <vector>
#include <cmath>

namespace ImageProcessing {

/**
 * @brief Gaussian blur filter implementation
 * Demonstrates inheritance and polymorphism
 */
class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(double sigma = 1.0, int kernel_size = 5);
    
    // Filter interface implementation
    std::unique_ptr<Image> apply(const Image& input) const override;
    std::string getName() const override { return "GaussianBlur"; }
    void setParameters(const std::map<std::string, double>& params) override;
    std::map<std::string, double> getParameters() const override;
    std::unique_ptr<Filter> clone() const override;
    
    // Specific methods
    void setSigma(double sigma);
    void setKernelSize(int size);
    double getSigma() const { return sigma_; }
    int getKernelSize() const { return kernel_size_; }

private:
    double sigma_;
    int kernel_size_;
    std::vector<std::vector<double>> kernel_;
    
    void generateKernel();
    double gaussianFunction(double x, double y) const;
};

} // namespace ImageProcessing
