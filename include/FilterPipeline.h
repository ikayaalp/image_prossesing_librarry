#pragma once

#include "Filter.h"
#include <vector>
#include <memory>
#include <string>

namespace ImageProcessing {

/**
 * @brief Filter pipeline for applying multiple filters in sequence
 * Demonstrates composition pattern and STL containers
 */
class FilterPipeline {
public:
    FilterPipeline() = default;
    ~FilterPipeline() = default;
    
    // Pipeline management
    void addFilter(std::unique_ptr<Filter> filter);
    void removeFilter(size_t index);
    void clear();
    
    // Apply pipeline
    std::unique_ptr<Image> apply(const Image& input) const;
    
    // Pipeline information
    size_t getFilterCount() const { return filters_.size(); }
    std::string getFilterName(size_t index) const;
    std::vector<std::string> getAllFilterNames() const;
    
    // Pipeline operations
    void swapFilters(size_t index1, size_t index2);
    void insertFilter(size_t index, std::unique_ptr<Filter> filter);
    
    // Clone pipeline
    std::unique_ptr<FilterPipeline> clone() const;

private:
    std::vector<std::unique_ptr<Filter>> filters_;
};

} // namespace ImageProcessing
