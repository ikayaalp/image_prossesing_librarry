#pragma once
#include <vector>
#include <string>
#include <memory>

namespace GorselIsleme {

class Image {
public:
    using Pixel = unsigned char;
    using PixelVector = std::vector<Pixel>;
    
    Image(int width = 0, int height = 0, int channels = 1);
    Image(const Image& other);
    Image& operator=(const Image& other);
    
    // Piksel erisim
    Pixel& at(int x, int y, int channel = 0);
    const Pixel& at(int x, int y, int channel = 0) const;
    
    // Bilgi fonksiyonlari
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    int getChannels() const { return channels_; }
    bool empty() const { return data_.empty(); }
    
    // Dosya islemleri
    bool save(const std::string& filename) const;
    static std::unique_ptr<Image> load(const std::string& filename);
    static std::unique_ptr<Image> loadJPEG(const std::string& filename);
    
    // Veri erisim
    const PixelVector& getData() const { return data_; }
    PixelVector& getData() { return data_; }
    
private:
    int width_;
    int height_;
    int channels_;
    PixelVector data_;
    
    bool isValidCoordinate(int x, int y, int channel) const;
    size_t calculateIndex(int x, int y, int channel) const;
};

} // namespace GorselIsleme