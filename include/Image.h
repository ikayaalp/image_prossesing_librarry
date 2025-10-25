#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <fstream>
#include <iostream>

namespace ImageProcessing {

/**
 * @brief Modern C++17/20 ile geliştirilmiş görüntü sınıfı
 * STL konteynerları ve algoritmaları kullanır
 */
class Image {
public:
    using Pixel = std::uint8_t;
    using PixelVector = std::vector<Pixel>;
    using Size = std::pair<int, int>;

    // Modern C++17 constructor'lar
    Image() = default;
    Image(int width, int height, int channels = 1);
    Image(const Image& other);
    Image(Image&& other) noexcept;
    
    // Destructor
    ~Image() = default;

    // Assignment operators
    Image& operator=(const Image& other);
    Image& operator=(Image&& other) noexcept;

    // Getters - Modern C++17 constexpr
    constexpr int getWidth() const noexcept { return width_; }
    constexpr int getHeight() const noexcept { return height_; }
    constexpr int getChannels() const noexcept { return channels_; }
    constexpr Size getSize() const noexcept { return {width_, height_}; }
    
    // Pixel erişimi - Modern C++17 optional
    Pixel& at(int x, int y, int channel = 0);
    const Pixel& at(int x, int y, int channel = 0) const;
    Pixel& operator()(int x, int y, int channel = 0);
    const Pixel& operator()(int x, int y, int channel = 0) const;

    // Modern C++17 optional ile güvenli erişim
    std::optional<Pixel> getPixel(int x, int y, int channel = 0) const;
    void setPixel(int x, int y, int channel, Pixel value);

    // Görüntü işlemleri
    void clear();
    void resize(int new_width, int new_height);
    bool empty() const noexcept;
    size_t getTotalPixels() const noexcept;
    
    // İstatistiksel bilgiler - STL algoritmaları
    std::optional<Pixel> getMinValue() const;
    std::optional<Pixel> getMaxValue() const;
    double getMeanValue() const;
    double getStandardDeviation() const;

    // Dosya işlemleri
    bool save(const std::string& filename) const;
    static std::unique_ptr<Image> load(const std::string& filename);
    
    // JPEG işlemleri
    bool saveJPEG(const std::string& filename, int quality = 95) const;
    static std::unique_ptr<Image> loadJPEG(const std::string& filename);

    // STL algoritmaları ile işlemler
    void normalize();
    void histogram(std::vector<int>& hist) const;
    
    // Modern C++17 features
    void applyFunction(std::function<Pixel(Pixel)> func);
    void applyKernel(const std::vector<std::vector<double>>& kernel);
    
    // Veri erişimi
    const PixelVector& getData() const { return data_; }
    PixelVector& getData() { return data_; }

private:
    int width_{0};
    int height_{0};
    int channels_{1};
    PixelVector data_;
    
    bool isValidCoordinate(int x, int y, int channel = 0) const;
    size_t calculateIndex(int x, int y, int channel = 0) const;
};

} // namespace ImageProcessing
