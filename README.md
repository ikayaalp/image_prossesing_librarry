# Gorsel Isleme Kutuphanesi

[![pipeline status](https://gitlab.com/your-username/gorselisleme/badges/main/pipeline.svg)](https://gitlab.com/your-username/gorselisleme/-/pipelines)
[![coverage report](https://gitlab.com/your-username/gorselisleme/badges/main/coverage.svg)](https://gitlab.com/your-username/gorselisleme/-/jobs)
[![build status](https://gitlab.com/your-username/gorselisleme/badges/main/build.svg)](https://gitlab.com/your-username/gorselisleme/-/pipelines)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)
[![SFML](https://img.shields.io/badge/SFML-2.5+-orange.svg)](https://www.sfml-dev.org/)

Modern C++17/20 özelliklerini, Nesne Yönelimli Programlama (OOP) prensiplerini ve Standart Şablon Kütüphanesi (STL) algoritmalarını kullanarak geliştirilmiş bir görüntü işleme kütüphanesidir.

## 🚀 Hızlı Başlangıç

### 1. Depoyu Klonlayın
```bash
git clone https://github.com/ikayaalp/image_prossesing_librarry.git
cd image_prossesing_librarry
```

### 2. Projeyi Derleyin
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### 3. Demo Uygulamasını Çalıştırın
```bash
./image_demo
```

### 4. Testleri Çalıştırın
```bash
ctest --output-on-failure
```

## 📖 Kullanım

### Temel Görüntü İşlemleri
```cpp
#include "Image.h"
#include "ImageViewer.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include <iostream>
#include <memory>

using namespace GorselIsleme;

int main() {
    ImageViewer viewer;

    // Görüntü yükle
    auto gorsel = Image::load("resim.pgm");
    if (!gorsel) {
        std::cerr << "Görüntü yüklenemedi!" << std::endl;
        return 1;
    }
    
    viewer.imshow("Orijinal Görüntü", *gorsel);

    // Filtreleri sırayla uygula
    GaussianBlur blur_filter(1.5, 3);
    auto bulanik_sonuc = blur_filter.apply(*gorsel);
    viewer.imshow("Bulanık Görüntü", *bulanik_sonuc);

    EdgeDetection kenar_filtresi;
    auto kenar_sonuc = kenar_filtresi.apply(*bulanik_sonuc);
    viewer.imshow("Kenar Tespiti", *kenar_sonuc);

    BrightnessAdjust parlaklik_filtresi(1.2);
    auto final_sonuc = parlaklik_filtresi.apply(*kenar_sonuc);
    viewer.imshow("Sonuç Görüntü", *final_sonuc);

    // Filtre pipeline'ı
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto pipeline_sonuc = pipeline.apply(*gorsel);
    viewer.imshow("Pipeline Çıktısı", *pipeline_sonuc);

    // Sonuçları kaydet
    final_sonuc->save("islenmis_görüntü.pgm");
    pipeline_sonuc->save("pipeline_cikti.pgm");

    viewer.waitKey(0);
    return 0;
}
```

### Sadece Kenar Bulma Programı
```bash
# Derleme
cd build
cmake ..
make kenar_bulma

# Çalıştırma
./kenar_bulma
```

## 🧪 Test Etme

### Tüm Testleri Çalıştırma
```bash
cd build
ctest --output-on-failure
```

### Test Kategorileri
- **Unit Tests**: Temel sınıf fonksiyonları
- **Filter Tests**: Filtre algoritmaları
- **Pipeline Tests**: Filtre zinciri
- **Integration Tests**: Tam entegrasyon testleri

## 🏗️ Proje Yapısı

```
ImageProcessingLib/
├── src/                    # Kaynak dosyaları (.cpp)
├── include/                # Başlık dosyaları (.h)
├── tests/                  # Birim ve entegrasyon testleri
├── CMakeLists.txt          # CMake build konfigürasyonu
├── .gitlab-ci.yml          # GitLab CI/CD pipeline tanımı
├── README.md               # Bu dosya
└── ... (diğer dosyalar)
```

## 🔧 Özellikler

### OOP Tasarımı
- **7 Sınıf**: Image, Filter, GaussianBlur, EdgeDetection, BrightnessAdjust, FilterPipeline, ImageViewer
- **Kalıtım**: Filter → GaussianBlur, EdgeDetection, BrightnessAdjust
- **Polimorfizm**: Virtual fonksiyonlar ve çok biçimlilik

### Modern C++ Özellikleri
- **C++17/20**: `std::unique_ptr`, `auto`, `constexpr`
- **STL Kullanımı**: `std::vector`, `std::map`, `std::transform`
- **Exception Safety**: Güvenli hata yönetimi

### Görüntü İşleme
- **Görüntü Yükleme/Kaydetme**: PGM format desteği
- **Filtreler**: Gaussian Blur, Kenar Tespiti, Parlaklık Ayarı
- **Filtre Pipeline**: Zincirleme filtre uygulaması
- **Görsel Gösterim**: SFML tabanlı ImageViewer

### CI/CD Pipeline
- **GitLab CI/CD**: Otomatik build ve test
- **Multi-Platform**: Linux ve Windows desteği
- **Automated Testing**: Unit ve integration testler
- **Code Quality**: Cppcheck ve Clang-format
- **Packaging**: Otomatik paket oluşturma

## 📊 Teknik Gereksinimler

### ✅ Minimum Teknik Gereksinimler
- ✅ **En az 5 sınıf**: 7 sınıf mevcut
- ✅ **Kalıtım ve polimorfizm**: Filter sınıf hiyerarşisi
- ✅ **Modern C++ özellikleri**: C++17/20 aktif kullanım
- ✅ **STL kullanımı**: Konteynerlar ve algoritmalar

### ✅ CI/CD Pipeline Gereksinimleri
- ✅ **GitLab Repository**: Public repository
- ✅ **GitLab CI/CD**: Otomatik build ve test pipeline
- ✅ **Automated Testing**: Unit testlerin CI/CD entegrasyonu
- ✅ **Build Badges**: README'de pipeline durumu
- ✅ **CMake**: Proje build sistemi
- ✅ **Multi-Platform**: Linux ve Windows desteği

## 📝 Lisans

Bu proje MIT lisansı altında lisanslanmıştır.