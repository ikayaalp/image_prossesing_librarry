#pragma once
#include "Filter.h"
#include <vector>
#include <memory>
#include <string>

namespace GorselIsleme {

class FilterPipeline {
public:
    FilterPipeline() = default;
    
    void addFilter(std::unique_ptr<Filter> filter);
    void removeFilter(size_t index);
    void clear();
    
    std::unique_ptr<Image> apply(const Image& input) const;
    
    std::string getFilterName(size_t index) const;
    std::vector<std::string> getAllFilterNames() const;
    size_t getFilterCount() const { return filters_.size(); }
    
    std::unique_ptr<FilterPipeline> clone() const;
    
private:
    std::vector<std::unique_ptr<Filter>> filters_;
};

} // namespace GorselIsleme