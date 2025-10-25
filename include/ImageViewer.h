#pragma once

#include "Image.h"
#include <string>
#include <memory>
#include <functional>

namespace ImageProcessing {

/**
 * @brief OpenCV imshow benzeri görüntü görüntüleme sınıfı
 * SFML kullanarak pencere tabanlı görüntü gösterimi sağlar
 */
class ImageViewer {
public:
    // Constructor/Destructor
    ImageViewer();
    ~ImageViewer();

    // Ana görüntüleme fonksiyonu - OpenCV imshow benzeri
    void imshow(const std::string& window_name, const Image& image);
    
    // Pencere yönetimi
    void waitKey(int delay = 0);
    void destroyWindow(const std::string& window_name);
    void destroyAllWindows();
    
    // Pencere durumu kontrolü
    bool isWindowOpen(const std::string& window_name) const;
    bool hasOpenWindows() const;
    
    // Görüntü güncelleme
    void updateImage(const std::string& window_name, const Image& image);
    
    // Pencere özellikleri
    void setWindowTitle(const std::string& window_name, const std::string& title);
    void resizeWindow(const std::string& window_name, int width, int height);
    
    // Event handling
    void setKeyCallback(const std::string& window_name, 
                       std::function<void(int)> callback);
    void setMouseCallback(const std::string& window_name,
                         std::function<void(int, int, int)> callback);

private:
    class WindowData;
    std::unique_ptr<WindowData> window_data_;
    
    // Internal helper functions
    void createWindow(const std::string& window_name, int width, int height);
    void handleEvents();
    void renderWindow(const std::string& window_name);
};

} // namespace ImageProcessing
