#pragma once
#include "Image.h"
#include <memory>
#include <string>
#include <map>

namespace GorselIsleme {

class Filter {
public:
    virtual ~Filter() = default;
    virtual std::unique_ptr<Image> apply(const Image& input) const = 0;
    virtual std::string getName() const = 0;
    virtual std::unique_ptr<Filter> clone() const = 0;
};

} // namespace GorselIsleme