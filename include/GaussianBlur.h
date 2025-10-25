#pragma once
#include "Filter.h"

namespace GorselIsleme {

class GaussianBlur : public Filter {
public:
    GaussianBlur(double sigma = 1.0, int kernel_size = 3);
    
    std::unique_ptr<Image> apply(const Image& input) const override;
    std::string getName() const override { return "GaussianBlur"; }
    std::unique_ptr<Filter> clone() const override;
    
    void setSigma(double sigma);
    void setKernelSize(int size);
    
private:
    double sigma_;
    int kernel_size_;
    std::vector<std::vector<double>> kernel_;
    
    void generateKernel();
    double gaussianFunction(double x, double y) const;
};

} // namespace GorselIsleme