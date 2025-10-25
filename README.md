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
- Python 3 + PIL (JPEG desteği için)

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

## 📖 Nasıl Kullanılır

### 1. Kendi Görüntünüzü Yükleyin
```cpp
#include "Image.h"
using namespace ImageProcessing;

// JPEG görüntünüzü yükleyin
auto my_image = Image::loadJPEG("kendi_resminiz.jpg");
if (my_image) {
    // Görüntü başarıyla yüklendi
    std::cout << "Görüntü boyutu: " << my_image->getWidth() << "x" << my_image->getHeight() << std::endl;
}
```

### 2. Filtreleri Sırayla Uygulayın
```cpp
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"

// Filtre sonuçlarını saklamak için değişkenler
std::unique_ptr<Image> blurred_result;
std::unique_ptr<Image> edge_result;
std::unique_ptr<Image> bright_result;

// 1. Gaussian Blur uygula
auto blur_filter = std::make_unique<GaussianBlur>(2.0, 5);
blurred_result = blur_filter->apply(*my_image);

// 2. Kenar tespiti uygula
auto edge_filter = std::make_unique<EdgeDetection>();
edge_result = edge_filter->apply(*my_image);

// 3. Parlaklık ayarla
auto brightness_filter = std::make_unique<BrightnessAdjust>(1.3);
bright_result = brightness_filter->apply(*my_image);
```

### 3. Görüntüleyici ile Sonuçları Görün
```cpp
#include "ImageViewer.h"

ImageViewer viewer;

// Her adımı görsel olarak göster
viewer.imshow("Orijinal", *my_image);
viewer.imshow("Bulanık", *blurred_result);
viewer.imshow("Kenarlar", *edge_result);
viewer.imshow("Parlak", *bright_result);

// Kullanıcı girişini bekle
viewer.waitKey(0);
```

### 4. Filtre Pipeline'ı Oluşturun
```cpp
#include "FilterPipeline.h"

// Pipeline oluştur
FilterPipeline pipeline;

// Filtreleri sırayla ekle
pipeline.addFilter(std::make_unique<GaussianBlur>(1.5, 3));
pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
pipeline.addFilter(std::make_unique<EdgeDetection>());

// Tüm pipeline'ı uygula
auto final_result = pipeline.apply(*my_image);
```

### 5. Sonuçları Kaydedin
```cpp
// Sonuçları dosyaya kaydet
my_image->save("orijinal.pgm");
blurred_result->save("bulanik.pgm");
edge_result->save("kenarlar.pgm");
final_result->save("pipeline_sonucu.pgm");
```

## 🎯 Demo Uygulaması

Demo uygulaması (`./image_demo`) şunları yapar:

1. **Görüntü Yükleme**: Kendi JPEG görüntünüzü yükler (yoksa test görüntüsü oluşturur)
2. **Filtre Uygulama**: Her filtreyi ayrı ayrı uygular ve gösterir
3. **Pipeline Oluşturma**: Filtreleri zincirler
4. **Görsel Gösterim**: Her adımı pencerede gösterir
5. **İstatistikler**: Görüntü bilgilerini raporlar
6. **Kaydetme**: Sonuçları PGM formatında kaydeder

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

- **Kendi Görüntünüzü Yükleyin**: JPEG desteği ile
- **Filtreleri Sırayla Uygulayın**: Her sonucu ayrı değişkende saklayın
- **Görsel Gösterim**: SFML ile pencerede görüntüleyin
- **Filtre Pipeline'ı**: Filtreleri zincirleyin
- **Modern C++17/20**: Smart pointers, STL, lambda expressions
- **OOP Tasarım**: Kalıtım, polimorfizm, kapsülleme
- **Kapsamlı Testler**: 60+ birim test

## 📄 Lisans

MIT Lisansı - detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

**Modern C++17/20 ile ❤️ ile yapıldı**