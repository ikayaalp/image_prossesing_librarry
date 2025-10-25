#pragma once

#include "Image.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace ImageProcessing {

/**
 * @brief Abstract base class for all image filters
 * Demonstrates polymorphism and inheritance
 */
class Filter {
public:
    virtual ~Filter() = default;
    
    /**
     * @brief Apply filter to image - polymorphic interface
     * @param input Input image
     * @return Filtered image
     */
    virtual std::unique_ptr<Image> apply(const Image& input) const = 0;
    
    /**
     * @brief Get filter name
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief Set filter parameters
     */
    virtual void setParameters(const std::map<std::string, double>& params) = 0;
    
    /**
     * @brief Get filter parameters
     */
    virtual std::map<std::string, double> getParameters() const = 0;
    
    /**
     * @brief Clone filter (virtual constructor pattern)
     */
    virtual std::unique_ptr<Filter> clone() const = 0;
};

} // namespace ImageProcessing
