#pragma once
#include "Image.h"
#include <string>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

namespace GorselIsleme {

class ImageViewer {
public:
    ImageViewer();
    ~ImageViewer();
    
    void imshow(const std::string& window_name, const Image& image);
    void waitKey(int delay_ms = 0);
    void destroyWindow(const std::string& window_name);
    void destroyAllWindows();
    
    bool isWindowOpen(const std::string& window_name) const;
    bool hasOpenWindows() const;
    
private:
    class WindowData;
    std::unique_ptr<WindowData> window_data_;
    
    void createWindow(const std::string& window_name, int width, int height);
    void handleEvents();
    void renderWindow(const std::string& window_name);
};

} // namespace GorselIsleme