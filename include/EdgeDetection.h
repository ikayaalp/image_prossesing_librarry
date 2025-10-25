#pragma once
#include "Filter.h"
#include <vector>

namespace GorselIsleme {

class EdgeDetection : public Filter {
public:
    enum Direction { Horizontal, Vertical, Both };
    
    EdgeDetection(Direction direction = Both);
    
    std::unique_ptr<Image> apply(const Image& input) const override;
    std::string getName() const override { return "EdgeDetection"; }
    std::unique_ptr<Filter> clone() const override;
    
    void setDirection(Direction direction);
    
private:
    Direction direction_;
    std::vector<std::vector<int>> sobel_x_;
    std::vector<std::vector<int>> sobel_y_;
    
    void initializeSobelKernels();
    double applyConvolution(const Image& image, int x, int y, 
                          const std::vector<std::vector<int>>& kernel) const;
};

} // namespace GorselIsleme