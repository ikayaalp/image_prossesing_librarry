# 🖼️ Modern C++ Görüntü İşleme Kütüphanesi

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)
[![SFML](https://img.shields.io/badge/SFML-2.5+-orange.svg)](https://www.sfml-dev.org/)

Modern C++17/20 özelliklerini kullanan, OOP prensipleri ile tasarlanmış görüntü işleme kütüphanesi.

## 🚀 Hızlı Başlangıç

### Gereksinimler
- C++17 uyumlu derleyici (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16+
- SFML 2.5+
- Google Test (testler için)

### Kurulum
```bash
# Repository'yi klonlayın
git clone https://github.com/ikayaalp/image_prossesing_librarry.git
cd image_prossesing_librarry

# Derleyin
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Demo'yu çalıştırın
./image_demo
```

## 📖 Kullanım

### Temel Görüntü İşlemleri
```cpp
#include "Image.h"
using namespace ImageProcessing;

// Görüntü oluştur
Image image(800, 600, 1);

// Piksel ayarla ve al
image.at(100, 100) = 255;
auto pixel = image.getPixel(100, 100);

// Görüntüyü kaydet
image.save("cikti.pgm");
```

### Filtre Uygulama
```cpp
#include "GaussianBlur.h"
#include "EdgeDetection.h"

// Filtre oluştur ve uygula
auto blur = std::make_unique<GaussianBlur>(2.0, 5);
auto result = blur->apply(image);

// Kenar tespiti
auto edges = std::make_unique<EdgeDetection>()->apply(image);
```

### Filtre Pipeline'ı
```cpp
#include "FilterPipeline.h"

FilterPipeline pipeline;
pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
pipeline.addFilter(std::make_unique<EdgeDetection>());

auto result = pipeline.apply(image);
```

### Görüntü Görüntüleme
```cpp
#include "ImageViewer.h"

ImageViewer viewer;
viewer.imshow("Görüntü", image);
viewer.waitKey(0); // Kullanıcı girişini bekle
```

## 🧪 Test Etme

```bash
# Tüm testleri çalıştır
ctest --output-on-failure

# Belirli testleri çalıştır
./build/tests/imageprocessing_test --gtest_filter="ImageTest.*"
```

## 🏗️ Proje Yapısı

```
├── src/                    # Kaynak kodlar
│   ├── Image.cpp          # Temel görüntü sınıfı
│   ├── GaussianBlur.cpp   # Gaussian bulanıklaştırma
│   ├── EdgeDetection.cpp  # Kenar tespiti
│   ├── BrightnessAdjust.cpp # Parlaklık ayarlama
│   ├── FilterPipeline.cpp # Filtre zincirleme
│   ├── ImageViewer.cpp    # SFML görüntü görüntüleyici
│   └── main.cpp          # Demo uygulaması
├── include/               # Başlık dosyaları
├── tests/                 # Birim testler
└── CMakeLists.txt         # Derleme konfigürasyonu
```

## 🔧 Özellikler

- **Modern C++17/20**: Smart pointers, STL, lambda expressions
- **OOP Tasarım**: Kalıtım, polimorfizm, kapsülleme
- **SFML Entegrasyonu**: Görüntü görüntüleme (OpenCV imshow benzeri)
- **Kapsamlı Testler**: 60+ birim test
- **Filtre Pipeline'ı**: Filtreleri zincirleme
- **Çoklu Platform**: Linux, Windows, macOS

## 📄 Lisans

MIT Lisansı - detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

**Modern C++17/20 ile ❤️ ile yapıldı**