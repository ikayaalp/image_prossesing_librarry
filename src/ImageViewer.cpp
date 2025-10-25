#include "ImageViewer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <memory>
#include <iostream>
#include <algorithm>

namespace ImageProcessing {

// Forward declaration for internal window data
class ImageViewer::WindowData {
public:
    std::map<std::string, std::unique_ptr<sf::RenderWindow>> windows;
    std::map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::map<std::string, std::function<void(int)>> key_callbacks;
    std::map<std::string, std::function<void(int, int, int)>> mouse_callbacks;
    std::map<std::string, std::string> window_titles;
};

// ImageViewer oluşturur (SFML pencereleri için)
ImageViewer::ImageViewer() : window_data_(std::make_unique<WindowData>()) {
    // SFML initialization is automatic
}

// ImageViewer yıkıcısı (tüm pencereleri kapatır)
ImageViewer::~ImageViewer() {
    destroyAllWindows();
}

// Görüntüyü pencerede gösterir (OpenCV imshow benzeri)
void ImageViewer::imshow(const std::string& window_name, const Image& image) {
    if (image.empty()) {
        std::cerr << "Error: Cannot display empty image" << std::endl;
        return;
    }

    int width = image.getWidth();
    int height = image.getHeight();
    
    // Create window if it doesn't exist
    if (window_data_->windows.find(window_name) == window_data_->windows.end()) {
        createWindow(window_name, width, height);
    }
    
    // Update the image in the window
    updateImage(window_name, image);
    
    // Handle events and render
    handleEvents();
    renderWindow(window_name);
}

// Kullanıcı girişini bekler (OpenCV waitKey benzeri)
void ImageViewer::waitKey(int delay) {
    if (delay == 0) {
        // Wait indefinitely until a key is pressed
        while (hasOpenWindows()) {
            handleEvents();
            for (auto& [name, window] : window_data_->windows) {
                if (window && window->isOpen()) {
                    renderWindow(name);
                }
            }
            
            // Check if any key is pressed
            for (auto& [name, window] : window_data_->windows) {
                if (window && window->isOpen()) {
                    sf::Event event;
                    while (window->pollEvent(event)) {
                        if (event.type == sf::Event::KeyPressed) {
                            return;
                        }
                        if (event.type == sf::Event::Closed) {
                            window->close();
                        }
                    }
                }
            }
        }
    } else {
        // Wait for specified milliseconds
        sf::Clock clock;
        while (clock.getElapsedTime().asMilliseconds() < delay && hasOpenWindows()) {
            handleEvents();
            for (auto& [name, window] : window_data_->windows) {
                if (window && window->isOpen()) {
                    renderWindow(name);
                }
            }
        }
    }
}

// Belirtilen pencereyi kapatır ve temizler
void ImageViewer::destroyWindow(const std::string& window_name) {
    auto window_it = window_data_->windows.find(window_name);
    if (window_it != window_data_->windows.end()) {
        if (window_it->second && window_it->second->isOpen()) {
            window_it->second->close();
        }
        window_data_->windows.erase(window_it);
    }
    
    auto texture_it = window_data_->textures.find(window_name);
    if (texture_it != window_data_->textures.end()) {
        window_data_->textures.erase(texture_it);
    }
    
    window_data_->key_callbacks.erase(window_name);
    window_data_->mouse_callbacks.erase(window_name);
    window_data_->window_titles.erase(window_name);
}

// Tüm pencereleri kapatır ve temizler
void ImageViewer::destroyAllWindows() {
    for (auto& [name, window] : window_data_->windows) {
        if (window && window->isOpen()) {
            window->close();
        }
    }
    window_data_->windows.clear();
    window_data_->textures.clear();
    window_data_->key_callbacks.clear();
    window_data_->mouse_callbacks.clear();
    window_data_->window_titles.clear();
}

// Belirtilen pencerenin açık olup olmadığını kontrol eder
bool ImageViewer::isWindowOpen(const std::string& window_name) const {
    auto it = window_data_->windows.find(window_name);
    return it != window_data_->windows.end() && 
           it->second && 
           it->second->isOpen();
}

// Herhangi bir açık pencere olup olmadığını kontrol eder
bool ImageViewer::hasOpenWindows() const {
    for (const auto& [name, window] : window_data_->windows) {
        if (window && window->isOpen()) {
            return true;
        }
    }
    return false;
}

// Mevcut pencereye yeni görüntü günceller
void ImageViewer::updateImage(const std::string& window_name, const Image& image) {
    if (image.empty()) {
        return;
    }

    // Create or update texture
    auto& texture = window_data_->textures[window_name];
    if (!texture) {
        texture = std::make_unique<sf::Texture>();
    }

    // Convert Image data to SFML format
    int width = image.getWidth();
    int height = image.getHeight();
    
    // Create pixel array for SFML (RGBA format)
    std::vector<sf::Uint8> pixels(width * height * 4);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixel_index = (y * width + x) * 4;
            Image::Pixel gray_value = image.at(x, y);
            
            // Convert grayscale to RGBA
            pixels[pixel_index] = gray_value;     // R
            pixels[pixel_index + 1] = gray_value; // G
            pixels[pixel_index + 2] = gray_value; // B
            pixels[pixel_index + 3] = 255;        // A (opaque)
        }
    }
    
    // Update texture
    texture->create(width, height);
    texture->update(pixels.data());
    
    // Resize window if needed
    auto window_it = window_data_->windows.find(window_name);
    if (window_it != window_data_->windows.end() && window_it->second) {
        window_it->second->setSize(sf::Vector2u(width, height));
    }
}

void ImageViewer::setWindowTitle(const std::string& window_name, const std::string& title) {
    window_data_->window_titles[window_name] = title;
    auto window_it = window_data_->windows.find(window_name);
    if (window_it != window_data_->windows.end() && window_it->second) {
        window_it->second->setTitle(title);
    }
}

void ImageViewer::resizeWindow(const std::string& window_name, int width, int height) {
    auto window_it = window_data_->windows.find(window_name);
    if (window_it != window_data_->windows.end() && window_it->second) {
        window_it->second->setSize(sf::Vector2u(width, height));
    }
}

// Pencere için tuş basma callback'i ayarlar
void ImageViewer::setKeyCallback(const std::string& window_name, 
                                std::function<void(int)> callback) {
    window_data_->key_callbacks[window_name] = callback;
}

// Pencere için fare callback'i ayarlar
void ImageViewer::setMouseCallback(const std::string& window_name,
                                 std::function<void(int, int, int)> callback) {
    window_data_->mouse_callbacks[window_name] = callback;
}

// Yeni pencere oluşturur (SFML RenderWindow)
void ImageViewer::createWindow(const std::string& window_name, int width, int height) {
    auto window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width, height), 
        window_name,
        sf::Style::Default
    );
    
    window_data_->windows[window_name] = std::move(window);
    window_data_->window_titles[window_name] = window_name;
}


// Tüm pencerelerdeki olayları işler (SFML event handling)
void ImageViewer::handleEvents() {
    for (auto& [name, window] : window_data_->windows) {
        if (!window || !window->isOpen()) {
            continue;
        }
        
        sf::Event event;
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;
                    
                case sf::Event::KeyPressed:
                    if (window_data_->key_callbacks.find(name) != window_data_->key_callbacks.end()) {
                        window_data_->key_callbacks[name](event.key.code);
                    }
                    break;
                    
                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
                    if (window_data_->mouse_callbacks.find(name) != window_data_->mouse_callbacks.end()) {
                        int button = (event.type == sf::Event::MouseButtonPressed) ? 1 : 0;
                        window_data_->mouse_callbacks[name](
                            event.mouseButton.x, 
                            event.mouseButton.y, 
                            button
                        );
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

// Belirtilen pencereyi render eder (görüntüyü çizer)
void ImageViewer::renderWindow(const std::string& window_name) {
    auto window_it = window_data_->windows.find(window_name);
    auto texture_it = window_data_->textures.find(window_name);
    
    if (window_it == window_data_->windows.end() || 
        texture_it == window_data_->textures.end() ||
        !window_it->second || 
        !texture_it->second) {
        return;
    }
    
    auto& window = window_it->second;
    auto& texture = texture_it->second;
    
    if (!window->isOpen()) {
        return;
    }
    
    // Clear window
    window->clear(sf::Color::Black);
    
    // Create sprite from texture
    sf::Sprite sprite(*texture);
    
    // Center the image in the window
    sf::Vector2u window_size = window->getSize();
    sf::Vector2u texture_size = texture->getSize();
    
    float scale_x = static_cast<float>(window_size.x) / texture_size.x;
    float scale_y = static_cast<float>(window_size.y) / texture_size.y;
    float scale = std::min(scale_x, scale_y);
    
    sprite.setScale(scale, scale);
    sprite.setPosition(
        (window_size.x - texture_size.x * scale) / 2,
        (window_size.y - texture_size.y * scale) / 2
    );
    
    // Draw the sprite
    window->draw(sprite);
    window->display();
}

} // namespace ImageProcessing
