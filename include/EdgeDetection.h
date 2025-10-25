#pragma once

#include "Filter.h"
#include <vector>

namespace ImageProcessing {

/**
 * @brief Edge detection filter using Sobel operator
 * Demonstrates inheritance and polymorphism
 */
class EdgeDetection : public Filter {
public:
    enum class Direction { Horizontal, Vertical, Both };
    
    explicit EdgeDetection(Direction direction = Direction::Both);
    
    // Filter interface implementation
    std::unique_ptr<Image> apply(const Image& input) const override;
    std::string getName() const override { return "EdgeDetection"; }
    void setParameters(const std::map<std::string, double>& params) override;
    std::map<std::string, double> getParameters() const override;
    std::unique_ptr<Filter> clone() const override;
    
    // Specific methods
    void setDirection(Direction direction);
    Direction getDirection() const { return direction_; }

private:
    Direction direction_;
    std::vector<std::vector<int>> sobel_x_;
    std::vector<std::vector<int>> sobel_y_;
    
    void initializeSobelKernels();
    double applyConvolution(const Image& image, int x, int y, 
                          const std::vector<std::vector<int>>& kernel) const;
};

} // namespace ImageProcessing
