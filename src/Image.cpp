#include "Image.h"
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <cstdlib>

namespace ImageProcessing {

// Belirtilen boyutlarda yeni bir görüntü oluşturur
Image::Image(int width, int height, int channels)
    : width_(width), height_(height), channels_(channels) {
    if (width <= 0 || height <= 0 || channels <= 0) {
        throw std::invalid_argument("Dimensions must be positive");
    }
    data_.resize(width * height * channels, 0);
}

// Başka bir görüntüyü kopyalayarak yeni görüntü oluşturur
Image::Image(const Image& other)
    : width_(other.width_), height_(other.height_), 
      channels_(other.channels_), data_(other.data_) {
}

// Başka bir görüntüyü taşıyarak yeni görüntü oluşturur (move constructor)
Image::Image(Image&& other) noexcept
    : width_(other.width_), height_(other.height_),
      channels_(other.channels_), data_(std::move(other.data_)) {
    other.width_ = other.height_ = other.channels_ = 0;
}

// Başka bir görüntüyü bu görüntüye kopyalar (assignment operator)
Image& Image::operator=(const Image& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        channels_ = other.channels_;
        data_ = other.data_;
    }
    return *this;
}

// Başka bir görüntüyü bu görüntüye taşır (move assignment operator)
Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        channels_ = other.channels_;
        data_ = std::move(other.data_);
        
        other.width_ = other.height_ = other.channels_ = 0;
    }
    return *this;
}

// Belirtilen koordinatlardaki piksel değerini döndürür (değiştirilebilir)
Image::Pixel& Image::at(int x, int y, int channel) {
    if (!isValidCoordinate(x, y, channel)) {
        throw std::out_of_range("Invalid coordinates or channel");
    }
    return data_[calculateIndex(x, y, channel)];
}

// Belirtilen koordinatlardaki piksel değerini döndürür (salt okunur)
const Image::Pixel& Image::at(int x, int y, int channel) const {
    if (!isValidCoordinate(x, y, channel)) {
        throw std::out_of_range("Invalid coordinates or channel");
    }
    return data_[calculateIndex(x, y, channel)];
}

// Belirtilen koordinatlardaki piksel değerini döndürür (operator() versiyonu)
Image::Pixel& Image::operator()(int x, int y, int channel) {
    return at(x, y, channel);
}

// Belirtilen koordinatlardaki piksel değerini döndürür (const operator() versiyonu)
const Image::Pixel& Image::operator()(int x, int y, int channel) const {
    return at(x, y, channel);
}

// Belirtilen koordinatlardaki piksel değerini güvenli şekilde döndürür (optional)
std::optional<Image::Pixel> Image::getPixel(int x, int y, int channel) const {
    if (!isValidCoordinate(x, y, channel)) {
        return std::nullopt;
    }
    return data_[calculateIndex(x, y, channel)];
}

// Belirtilen koordinatlardaki piksel değerini ayarlar
void Image::setPixel(int x, int y, int channel, Pixel value) {
    if (isValidCoordinate(x, y, channel)) {
        data_[calculateIndex(x, y, channel)] = value;
    }
}

// Görüntüdeki tüm pikselleri sıfırlar (siyah yapar)
void Image::clear() {
    std::fill(data_.begin(), data_.end(), 0);
}

// Görüntünün boyutunu değiştirir
void Image::resize(int new_width, int new_height) {
    if (new_width <= 0 || new_height <= 0) {
        throw std::invalid_argument("New dimensions must be positive");
    }
    
    width_ = new_width;
    height_ = new_height;
    data_.resize(width_ * height_ * channels_, 0);
}

// Görüntünün boş olup olmadığını kontrol eder
bool Image::empty() const noexcept {
    return data_.empty();
}

// Görüntüdeki toplam piksel sayısını döndürür
size_t Image::getTotalPixels() const noexcept {
    return data_.size();
}

// Görüntüdeki en küçük piksel değerini döndürür
std::optional<Image::Pixel> Image::getMinValue() const {
    if (data_.empty()) {
        return std::nullopt;
    }
    return *std::min_element(data_.begin(), data_.end());
}

// Görüntüdeki en büyük piksel değerini döndürür
std::optional<Image::Pixel> Image::getMaxValue() const {
    if (data_.empty()) {
        return std::nullopt;
    }
    return *std::max_element(data_.begin(), data_.end());
}

// Görüntüdeki piksel değerlerinin ortalamasını hesaplar
double Image::getMeanValue() const {
    if (data_.empty()) {
        return 0.0;
    }
    return std::accumulate(data_.begin(), data_.end(), 0.0) / data_.size();
}

// Görüntüdeki piksel değerlerinin standart sapmasını hesaplar
double Image::getStandardDeviation() const {
    if (data_.empty()) {
        return 0.0;
    }
    
    double mean = getMeanValue();
    double sum_squared_diff = std::accumulate(data_.begin(), data_.end(), 0.0,
        [mean](double sum, Pixel pixel) {
            double diff = static_cast<double>(pixel) - mean;
            return sum + diff * diff;
        });
    
    return std::sqrt(sum_squared_diff / data_.size());
}

// Görüntüyü PGM formatında dosyaya kaydeder
bool Image::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Simple PGM format
    file << "P5\n" << width_ << " " << height_ << "\n255\n";
    file.write(reinterpret_cast<const char*>(data_.data()), data_.size());
    
    return file.good();
}

// PGM formatındaki dosyadan görüntü yükler
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
    file.ignore(); // Skip newline
    
    auto image = std::make_unique<Image>(width, height);
    file.read(reinterpret_cast<char*>(image->data_.data()), 
              image->data_.size());
    
    return file.good() ? std::move(image) : nullptr;
}

// Görüntüyü 0-255 aralığına normalize eder (kontrast artırır)
void Image::normalize() {
    if (data_.empty()) {
        return;
    }
    
    auto [min_val, max_val] = std::minmax_element(data_.begin(), data_.end());
    Pixel min_pixel = *min_val;
    Pixel max_pixel = *max_val;
    
    if (max_pixel == min_pixel) {
        return; // All pixels are the same
    }
    
    double scale = 255.0 / (max_pixel - min_pixel);
    std::transform(data_.begin(), data_.end(), data_.begin(),
        [min_pixel, scale](Pixel pixel) {
            return static_cast<Pixel>(std::round((pixel - min_pixel) * scale));
        });
}

// Görüntünün histogramını hesaplar (piksel değerlerinin dağılımı)
void Image::histogram(std::vector<int>& hist) const {
    hist.assign(256, 0);
    std::for_each(data_.begin(), data_.end(),
        [&hist](Pixel pixel) {
            ++hist[pixel];
        });
}

// Her piksele belirtilen fonksiyonu uygular (lambda fonksiyonları için)
void Image::applyFunction(std::function<Pixel(Pixel)> func) {
    std::transform(data_.begin(), data_.end(), data_.begin(), func);
}

// Görüntüye konvolüsyon kernel'i uygular (filtreleme için)
void Image::applyKernel(const std::vector<std::vector<double>>& kernel) {
    if (kernel.empty() || kernel[0].empty()) {
        return;
    }
    
    int kernel_size = kernel.size();
    int half_kernel = kernel_size / 2;
    
    PixelVector new_data = data_;
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            double sum = 0.0;
            double weight_sum = 0.0;
            
            for (int ky = 0; ky < kernel_size; ++ky) {
                for (int kx = 0; kx < kernel_size; ++kx) {
                    int nx = x + kx - half_kernel;
                    int ny = y + ky - half_kernel;
                    
                    if (nx >= 0 && nx < width_ && ny >= 0 && ny < height_) {
                        double weight = kernel[ky][kx];
                        sum += at(nx, ny) * weight;
                        weight_sum += weight;
                    }
                }
            }
            
            if (weight_sum > 0) {
                new_data[calculateIndex(x, y)] = static_cast<Pixel>(
                    std::round(sum / weight_sum));
            }
        }
    }
    
    data_ = std::move(new_data);
}

// Koordinatların geçerli olup olmadığını kontrol eder
bool Image::isValidCoordinate(int x, int y, int channel) const {
    return x >= 0 && x < width_ && 
           y >= 0 && y < height_ && 
           channel >= 0 && channel < channels_;
}

// 2D koordinatları 1D dizi indeksine dönüştürür
size_t Image::calculateIndex(int x, int y, int channel) const {
    return (y * width_ + x) * channels_ + channel;
}

// Görüntüyü JPEG formatında dosyaya kaydeder (Python PIL kullanarak)
bool Image::saveJPEG(const std::string& filename, int quality) const {
    // Geçici PGM dosyası oluştur
    std::string temp_pgm = filename + ".temp.pgm";
    if (!save(temp_pgm)) {
        return false;
    }
    
    // Python ile JPEG'e dönüştür
    std::string command = "python3 -c \""
        "from PIL import Image; "
        "img = Image.open('" + temp_pgm + "'); "
        "img.save('" + filename + "', 'JPEG', quality=" + std::to_string(quality) + "); "
        "print('JPEG saved')\"";
    
    int result = std::system(command.c_str());
    
    // Geçici dosyayı sil
    std::remove(temp_pgm.c_str());
    
    return result == 0;
}

// JPEG formatındaki dosyadan görüntü yükler (Python PIL kullanarak)
std::unique_ptr<Image> Image::loadJPEG(const std::string& filename) {
    // Geçici PGM dosyası oluştur
    std::string temp_pgm = filename + ".temp.pgm";
    
    // Python ile PGM'e dönüştür
    std::string command = "python3 -c \""
        "from PIL import Image; "
        "import numpy as np; "
        "img = Image.open('" + filename + "'); "
        "img_gray = img.convert('L'); "
        "img_array = np.array(img_gray); "
        "with open('" + temp_pgm + "', 'wb') as f: "
        "f.write(b'P5\\n'); "
        "f.write(f'{img_array.shape[1]} {img_array.shape[0]}\\n'.encode()); "
        "f.write(b'255\\n'); "
        "f.write(img_array.tobytes()); "
        "print('PGM created')\"";
    
    int result = std::system(command.c_str());
    if (result != 0) {
        return nullptr;
    }
    
    // PGM dosyasını yükle
    auto image = load(temp_pgm);
    
    // Geçici dosyayı sil
    std::remove(temp_pgm.c_str());
    
    return image;
}

} // namespace ImageProcessing
