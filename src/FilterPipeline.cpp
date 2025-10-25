#include "FilterPipeline.h"
#include <stdexcept>

namespace GorselIsleme {

void FilterPipeline::addFilter(std::unique_ptr<Filter> filter) {
    if (!filter) {
        throw std::invalid_argument("Filtre bos olamaz");
    }
    filters_.push_back(std::move(filter));
}

void FilterPipeline::removeFilter(size_t index) {
    if (index >= filters_.size()) {
        throw std::out_of_range("Gecersiz filtre indeksi");
    }
    filters_.erase(filters_.begin() + index);
}

void FilterPipeline::clear() {
    filters_.clear();
}

std::unique_ptr<Image> FilterPipeline::apply(const Image& input) const {
    if (filters_.empty()) {
        return std::make_unique<Image>(input);
    }
    
    std::unique_ptr<Image> current = std::make_unique<Image>(input);
    
    for (const auto& filter : filters_) {
        current = filter->apply(*current);
    }
    
    return current;
}

std::string FilterPipeline::getFilterName(size_t index) const {
    if (index >= filters_.size()) {
        throw std::out_of_range("Gecersiz filtre indeksi");
    }
    return filters_[index]->getName();
}

std::vector<std::string> FilterPipeline::getAllFilterNames() const {
    std::vector<std::string> names;
    for (const auto& filter : filters_) {
        names.push_back(filter->getName());
    }
    return names;
}

std::unique_ptr<FilterPipeline> FilterPipeline::clone() const {
    auto cloned_pipeline = std::make_unique<FilterPipeline>();
    
    for (const auto& filter : filters_) {
        cloned_pipeline->addFilter(filter->clone());
    }
    
    return cloned_pipeline;
}

} // namespace GorselIsleme