#include "FilterPipeline.h"
#include <stdexcept>

namespace ImageProcessing {

// Filtre pipeline'ına yeni filtre ekler
void FilterPipeline::addFilter(std::unique_ptr<Filter> filter) {
    if (!filter) {
        throw std::invalid_argument("Filter cannot be null");
    }
    filters_.push_back(std::move(filter));
}

// Belirtilen indeksteki filtreyi pipeline'dan çıkarır
void FilterPipeline::removeFilter(size_t index) {
    if (index >= filters_.size()) {
        throw std::out_of_range("Filter index out of range");
    }
    filters_.erase(filters_.begin() + index);
}

// Pipeline'daki tüm filtreleri temizler
void FilterPipeline::clear() {
    filters_.clear();
}

// Pipeline'daki tüm filtreleri sırayla uygular
std::unique_ptr<Image> FilterPipeline::apply(const Image& input) const {
    if (filters_.empty()) {
        return std::make_unique<Image>(input);
    }
    
    std::unique_ptr<Image> result = filters_[0]->apply(input);
    
    for (size_t i = 1; i < filters_.size(); ++i) {
        result = filters_[i]->apply(*result);
    }
    
    return result;
}

// Belirtilen indeksteki filtrenin adını döndürür
std::string FilterPipeline::getFilterName(size_t index) const {
    if (index >= filters_.size()) {
        throw std::out_of_range("Filter index out of range");
    }
    return filters_[index]->getName();
}

// Pipeline'daki tüm filtrelerin adlarını döndürür
std::vector<std::string> FilterPipeline::getAllFilterNames() const {
    std::vector<std::string> names;
    names.reserve(filters_.size());
    
    for (const auto& filter : filters_) {
        names.push_back(filter->getName());
    }
    
    return names;
}

// İki filtrenin yerini değiştirir
void FilterPipeline::swapFilters(size_t index1, size_t index2) {
    if (index1 >= filters_.size() || index2 >= filters_.size()) {
        throw std::out_of_range("Filter index out of range");
    }
    std::swap(filters_[index1], filters_[index2]);
}

// Belirtilen indekse yeni filtre ekler
void FilterPipeline::insertFilter(size_t index, std::unique_ptr<Filter> filter) {
    if (!filter) {
        throw std::invalid_argument("Filter cannot be null");
    }
    if (index > filters_.size()) {
        throw std::out_of_range("Filter index out of range");
    }
    filters_.insert(filters_.begin() + index, std::move(filter));
}

// Pipeline'ın bir kopyasını oluşturur (tüm filtrelerle birlikte)
std::unique_ptr<FilterPipeline> FilterPipeline::clone() const {
    auto cloned_pipeline = std::make_unique<FilterPipeline>();
    
    for (const auto& filter : filters_) {
        cloned_pipeline->addFilter(filter->clone());
    }
    
    return cloned_pipeline;
}

} // namespace ImageProcessing
