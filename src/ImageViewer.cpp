#include "ImageViewer.h"
#include <iostream>
#include <algorithm>

namespace GorselIsleme {

class ImageViewer::WindowData {
public:
    std::map<std::string, std::unique_ptr<sf::RenderWindow>> windows;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
};

ImageViewer::ImageViewer() : window_data_(std::make_unique<WindowData>()) {
}

ImageViewer::~ImageViewer() {
    destroyAllWindows();
}

void ImageViewer::goster(const std::string& window_name, const Image& image) {
    if (image.empty()) {
        std::cerr << "Bos gorsel gosterilemez" << std::endl;
        return;
    }
    
    if (window_data_->windows.find(window_name) == window_data_->windows.end()) {
        createWindow(window_name, image.getWidth(), image.getHeight());
    }
    
    sf::Texture& texture = window_data_->textures[window_name];
    sf::Sprite& sprite = window_data_->sprites[window_name];
    
    texture.create(image.getWidth(), image.getHeight());
    
    std::vector<sf::Uint8> pixels;
    pixels.reserve(image.getWidth() * image.getHeight() * 4);
    
    for (int y = 0; y < image.getHeight(); ++y) {
        for (int x = 0; x < image.getWidth(); ++x) {
            unsigned char gray = image.at(x, y);
            pixels.push_back(gray);
            pixels.push_back(gray);
            pixels.push_back(gray);
            pixels.push_back(255);
        }
    }
    
    texture.update(pixels.data());
    sprite.setTexture(texture);
    
    renderWindow(window_name);
}

void ImageViewer::waitKey(int delay_ms) {
    if (delay_ms > 0) {
        sf::sleep(sf::milliseconds(delay_ms));
    } else {
        while (hasOpenWindows()) {
            handleEvents();
            sf::sleep(sf::milliseconds(16));
        }
    }
}

void ImageViewer::destroyWindow(const std::string& window_name) {
    auto it = window_data_->windows.find(window_name);
    if (it != window_data_->windows.end()) {
        it->second->close();
        window_data_->windows.erase(it);
        window_data_->textures.erase(window_name);
        window_data_->sprites.erase(window_name);
    }
}

void ImageViewer::destroyAllWindows() {
    for (auto& pair : window_data_->windows) {
        pair.second->close();
    }
    window_data_->windows.clear();
    window_data_->textures.clear();
    window_data_->sprites.clear();
}

bool ImageViewer::isWindowOpen(const std::string& window_name) const {
    auto it = window_data_->windows.find(window_name);
    return it != window_data_->windows.end() && it->second->isOpen();
}

bool ImageViewer::hasOpenWindows() const {
    for (const auto& pair : window_data_->windows) {
        if (pair.second->isOpen()) {
            return true;
        }
    }
    return false;
}

void ImageViewer::createWindow(const std::string& window_name, int width, int height) {
    auto window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width, height), window_name);
    window_data_->windows[window_name] = std::move(window);
}

void ImageViewer::handleEvents() {
    for (auto& pair : window_data_->windows) {
        sf::Event event;
        while (pair.second->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                pair.second->close();
            }
        }
    }
}

void ImageViewer::renderWindow(const std::string& window_name) {
    auto it = window_data_->windows.find(window_name);
    if (it != window_data_->windows.end() && it->second->isOpen()) {
        it->second->clear();
        it->second->draw(window_data_->sprites[window_name]);
        it->second->display();
    }
}

} // namespace GorselIsleme