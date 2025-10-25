#include "Image.h"
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <cstdlib>
#include <sstream>

namespace GorselIsleme {

Image::Image(int width, int height, int channels)
    : width_(width), height_(height), channels_(channels) {
    if (width <= 0 || height <= 0 || channels <= 0) {
        throw std::invalid_argument("Gecersiz boyutlar");
    }
    data_.resize(width * height * channels, 0);
}

Image::Image(const Image& other)
    : width_(other.width_), height_(other.height_), 
      channels_(other.channels_), data_(other.data_) {
}

Image& Image::operator=(const Image& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        channels_ = other.channels_;
        data_ = other.data_;
    }
    return *this;
}

Image::Pixel& Image::at(int x, int y, int channel) {
    if (!isValidCoordinate(x, y, channel)) {
        throw std::out_of_range("Gecersiz koordinatlar");
    }
    return data_[calculateIndex(x, y, channel)];
}

const Image::Pixel& Image::at(int x, int y, int channel) const {
    if (!isValidCoordinate(x, y, channel)) {
        throw std::out_of_range("Gecersiz koordinatlar");
    }
    return data_[calculateIndex(x, y, channel)];
}

bool Image::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file << "P5\n" << width_ << " " << height_ << "\n255\n";
    file.write(reinterpret_cast<const char*>(data_.data()), data_.size());
    
    return file.good();
}

std::unique_ptr<Image> Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return nullptr;
    }
    
    std::string format;
    file >> format;
    if (format != "P5") {
        return nullptr;
    }
    
    int width, height, max_value;
    file >> width >> height >> max_value;
    file.ignore();
    
    auto image = std::make_unique<Image>(width, height);
    file.read(reinterpret_cast<char*>(image->data_.data()), image->data_.size());
    
    return file.good() ? std::move(image) : nullptr;
}

std::unique_ptr<Image> Image::loadJPEG(const std::string& filename) {
    // Python PIL kullanarak JPEG'i PGM'e çevir
    std::string temp_pgm = "temp_" + filename.substr(0, filename.find_last_of('.')) + ".pgm";
    
    std::stringstream cmd;
    cmd << "python3 -c \""
        << "from PIL import Image; "
        << "img = Image.open('" << filename << "').convert('L'); "
        << "img.save('" << temp_pgm << "', 'PPM'); "
        << "print('JPEG converted to PGM')\"";
    
    int result = std::system(cmd.str().c_str());
    if (result != 0) {
        return nullptr;
    }
    
    // PGM dosyasını yükle
    auto image = load(temp_pgm);
    
    // Geçici dosyayı sil
    std::remove(temp_pgm.c_str());
    
    return image;
}

bool Image::isValidCoordinate(int x, int y, int channel) const {
    return x >= 0 && x < width_ && 
           y >= 0 && y < height_ && 
           channel >= 0 && channel < channels_;
}

size_t Image::calculateIndex(int x, int y, int channel) const {
    return (y * width_ + x) * channels_ + channel;
}

} // namespace GorselIsleme