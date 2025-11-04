# Görselİşleme Kütüphanesi - Sunum Slaytları

## SLİDE 1: BAŞLIK SAYFASI
**Başlık:** Görselİşleme Kütüphanesi
**Alt Başlık:** C++ ile Nesne Yönelimli Görüntü İşleme Projesi
**Alt satır:** OOP (Object-Oriented Programming) Projesi
**Tarih/Kurs:** [İsteğe bağlı]

---

## SLİDE 2: GİRİŞ VE AMAÇ
**Başlık:** Giriş ve Proje Amacı

**Giriş Metni:**
Görüntü işleme, modern bilgisayar bilimlerinin en önemli alanlarından biridir. Bu proje, C++ programlama dili ve nesne yönelimli programlama (OOP) prensiplerini kullanarak, modüler ve genişletilebilir bir görüntü işleme kütüphanesi geliştirmeyi amaçlamaktadır.

**Proje Amacı:**
- ✅ **Nesne Yönelimli Programlama** prensiplerini pratik olarak uygulamak
- ✅ **Modüler ve genişletilebilir** bir kütüphane mimarisi oluşturmak
- ✅ **Tasarım desenlerini** (Strategy, Composite) kullanarak esnek bir yapı kurmak
- ✅ **Görüntü işleme algoritmalarını** (blur, kenar tespiti, parlaklık ayarı) uygulamak
- ✅ **Test-driven development** yaklaşımı ile güvenilir kod yazmak
- ✅ **CMake** build sistemi ve **SFML** görselleştirme kütüphanesi ile modern C++ projesi geliştirmek

**Hedef Kitle:** Bu proje, OOP prensiplerini öğrenmek ve görüntü işleme alanına giriş yapmak isteyen geliştiriciler için eğitici bir örnektir.

---

## SLİDE 3: MİMARİ GENEL BAKIŞ
**Başlık:** Proje Mimarisi

**İçerik:**
```
┌─────────────────┐
│     Image       │ ← Temel veri yapısı
└────────┬────────┘
         │
    ┌────┴────┐
    │ Filter  │ ← Soyut arayüz
    └────┬────┘
         │
    ┌────┼────────────────┐
    │    │                │
┌───▼──┐ ┌───▼──┐ ┌──────▼──────┐
│Gaussian│ │Edge │ │Brightness   │
│ Blur   │ │Detect│ │Adjust      │
└────────┘ └─────┘ └─────────────┘
    │
┌───▼───────────┐
│FilterPipeline │ ← Filtre zinciri
└───────────────┘
    │
┌───▼───────────┐
│ ImageViewer   │ ← Görselleştirme
└───────────────┘
```

**Açıklama:**
- Modüler ve genişletilebilir tasarım
- Strategy Pattern kullanımı
- Her sınıf tek bir sorumluluğa sahip (Single Responsibility)

---

## SLİDE 4: SINIFLAR VE İŞLEVLERİ
**Başlık:** Sınıflar ve İşlevleri - Özet

**1. Image Sınıfı**
- **İşlev:** Görüntü verilerini saklar ve yönetir
- **Özellikler:** PGM/JPEG okuma-yazma, piksel matrisi yönetimi, güvenli erişim

**2. Filter Sınıfı (Soyut Arayüz)**
- **İşlev:** Tüm filtrelerin ortak arayüzünü tanımlar
- **Özellikler:** Strategy Pattern, genişletilebilir yapı

**3. GaussianBlur Sınıfı**
- **İşlev:** Görüntüyü yumuşatır ve gürültüyü azaltır
- **Özellikler:** Sigma ve kernel boyutu parametreleri, Gaussian kernel

**4. EdgeDetection Sınıfı**
- **İşlev:** Görüntüdeki kenarları tespit eder ve vurgular
- **Özellikler:** Sobel operatörü, yatay/dikey/ikisi birden seçenekleri

**5. BrightnessAdjust Sınıfı**
- **İşlev:** Görüntünün parlaklığını artırır veya azaltır
- **Özellikler:** Brightness factor parametresi, piksel ölçekleme

**6. FilterPipeline Sınıfı**
- **İşlev:** Birden fazla filtreyi sırayla uygular
- **Özellikler:** Filtre ekleme/çıkarma, zincirleme işleme, Composite Pattern

**7. ImageViewer Sınıfı**
- **İşlev:** Görüntüleri ekranda gösterir
- **Özellikler:** SFML entegrasyonu, pencere yönetimi, çoklu pencere desteği

---

## SLİDE 5: Sınıf 1 - Image
**Başlık:** Image Sınıfı - Temel Veri Yapısı

**Görev:**
Görüntü verilerini saklar ve yönetir

**Özellikler:**
- 📁 PGM ve JPEG dosya okuma/yazma
- 🔢 Piksel matrisi yönetimi (width, height, channels)
- 🔒 Güvenli piksel erişimi (sınır kontrolü)
- 📊 Veri manipülasyonu

**Kod Örneği:**
```cpp
auto gorsel = Image::load("resim.pgm");
int w = gorsel->getWidth();
gorsel->at(x, y) = 255;  // Piksel değeri
gorsel->save("sonuc.pgm");
```

---

## SLİDE 6: Sınıf 2 - Filter
**Başlık:** Filter Sınıfı - Soyut Arayüz

**Görev:**
Tüm filtrelerin ortak arayüzünü tanımlar

**Tasarım Deseni:** Strategy Pattern

**Metodlar:**
- `apply()` → Görüntüye filtre uygular
- `getName()` → Filtre adını döndürür
- `clone()` → Filtrenin kopyasını oluşturur

**Avantaj:**
- Yeni filtre eklemek kolay
- Tüm filtreler aynı şekilde kullanılabilir
- Kod tekrarı yok

---

## SLİDE 7: Sınıf 3 - GaussianBlur
**Başlık:** GaussianBlur - Bulanıklaştırma Filtresi

**Görev:**
Görüntüyü yumuşatır ve gürültüyü azaltır

**Parametreler:**
- `sigma` (varsayılan: 1.0) → Bulanıklık yoğunluğu
- `kernel_size` (varsayılan: 3) → Kernel boyutu

**Kullanım:**
```cpp
GaussianBlur blur(10.0, 9);
auto sonuc = blur.apply(*gorsel);
```

**Görsel:** Önce → Sonra (bulanık görüntü)

---

## SLİDE 8: Sınıf 4 - EdgeDetection
**Başlık:** EdgeDetection - Kenar Tespiti

**Görev:**
Görüntüdeki kenarları tespit eder ve vurgular

**Algoritma:** Sobel Operatörü

**Yön Seçenekleri:**
- `Horizontal` → Sadece yatay kenarlar
- `Vertical` → Sadece dikey kenarlar
- `Both` → Tüm kenarlar (varsayılan)

**Kullanım:**
```cpp
EdgeDetection kenar;
auto kenar_sonuc = kenar.apply(*gorsel);
```

**Görsel:** Orijinal → Kenar tespiti sonucu

---

## SLİDE 9: Sınıf 5 - BrightnessAdjust
**Başlık:** BrightnessAdjust - Parlaklık Ayarı

**Görev:**
Görüntünün parlaklığını artırır veya azaltır

**Parametre:**
- `brightness_factor` (varsayılan: 1.0)
  - = 1.0 → Değişiklik yok
  - < 1.0 → Koyulaştır (örn: 0.5)
  - > 1.0 → Aydınlat (örn: 1.5)

**Kullanım:**
```cpp
BrightnessAdjust parlaklik(0.1);  // Koyulaştır
auto parlak_sonuc = parlaklik.apply(*gorsel);
```

**Görsel:** Normal → Koyu/Aydınlık versiyon

---

## SLİDE 10: Sınıf 6 - FilterPipeline
**Başlık:** FilterPipeline - Filtre Zinciri

**Görev:**
Birden fazla filtreyi sırayla uygular

**Tasarım Deseni:** Composite Pattern

**Özellikler:**
- Filtre ekleme/çıkarma
- Sıralı işleme (ilk filtre çıktısı → ikinci filtre girişi)
- Pipeline kopyalama

**Kullanım:**
```cpp
FilterPipeline pipeline;
pipeline.addFilter(std::make_unique<GaussianBlur>(1.5, 5));
pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.3));
pipeline.addFilter(std::make_unique<EdgeDetection>());

auto sonuc = pipeline.apply(*gorsel);
```

**Görsel:** Pipeline akış şeması

---

## SLİDE 11: Sınıf 7 - ImageViewer
**Başlık:** ImageViewer - Görselleştirme

**Görev:**
Görüntüleri ekranda gösterir

**Teknoloji:** SFML (Simple and Fast Multimedia Library)

**Özellikler:**
- Pencere yönetimi
- Çoklu pencere desteği
- Kullanıcı etkileşimi (waitKey)

**Kullanım:**
```cpp
ImageViewer viewer;
viewer.goster("Orijinal", *gorsel);
viewer.waitKey(0);  // Kullanıcı girişi bekle
```

---

## SLİDE 12: KULLANIM ÖRNEĞİ
**Başlık:** Kod Örneği - Tam Kullanım

**Kod:**
```cpp
#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include "ImageViewer.h"

using namespace GorselIsleme;

int main() {
    // 1. Görüntü yükle
    auto gorsel = Image::load("resim.pgm");
    
    // 2. Filtreler oluştur
    GaussianBlur blur(10.0, 9);
    EdgeDetection kenar;
    BrightnessAdjust parlaklik(0.1);
    
    // 3. Filtreleri uygula
    auto bulanik = blur.apply(*gorsel);
    auto kenar_sonuc = kenar.apply(*bulanik);
    auto parlak = parlaklik.apply(*gorsel);
    
    // 4. Sonuçları göster
    ImageViewer viewer;
    viewer.goster("Orijinal", *gorsel);
    viewer.goster("Bulanik", *bulanik);
    viewer.goster("Kenar", *kenar_sonuc);
    viewer.goster("Parlak", *parlak);
    viewer.waitKey(0);
    
    return 0;
}
```

---

## SLİDE 13: ÖZELLİKLER
**Başlık:** Kütüphane Özellikleri

**✅ Dosya Formatları:**
- PGM (Portable Graymap) - Okuma/Yazma
- JPEG - Okuma desteği

**✅ Filtreler:**
- Gaussian Blur (Bulanıklaştırma)
- Edge Detection (Kenar Tespiti)
- Brightness Adjust (Parlaklık)

**✅ İşlevsellik:**
- Filtre Pipeline (Zincirleme)
- Görselleştirme (SFML)
- Unit Testler (Google Test)
- Entegrasyon Testleri

**✅ Teknik:**
- C++17 standardı
- CMake build sistemi
- Modüler mimari

---

## SLİDE 14: TASARIM PRENSİPLERİ
**Başlık:** OOP Tasarım Prensipleri

**✅ Kullanılan Prensipler:**
1. **Single Responsibility** → Her sınıf tek bir görev
2. **Open/Closed** → Genişletmeye açık, değişikliğe kapalı
3. **Liskov Substitution** → Alt sınıflar üst sınıfı değiştirebilir
4. **Interface Segregation** → Filter arayüzü minimal
5. **Dependency Inversion** → Soyutlamalara bağımlılık

**✅ Tasarım Desenleri:**
- Strategy Pattern (Filter hiyerarşisi)
- Composite Pattern (FilterPipeline)
- Factory Pattern (Image::load)

---

## SLİDE 15: TEST ALTYAPISI
**Başlık:** Test ve Kalite Güvencesi

**Test Dosyaları:**
- `test_image.cpp` → Image sınıfı testleri
- `test_filters.cpp` → Filtre testleri
- `test_pipeline.cpp` → Pipeline testleri
- `test_integration.cpp` → Entegrasyon testleri

**Test Framework:** Google Test (GTest)

**Çalıştırma:**
```bash
cd build
ctest --output-on-failure
```

**Kapsam:**
- Unit testler
- Entegrasyon testleri
- Performans testleri
- Dosya I/O testleri

---

## SLİDE 16: DERLEME VE ÇALIŞTIRMA
**Başlık:** Kurulum ve Kullanım

**Kurulum:**
```bash
cd ImageProcessingLib
mkdir build && cd build
cmake ..
make
```

**Çalıştırma:**
```bash
./image_demo
# veya
./gorselisleme_test  # Testler için
```

**Bağımlılıklar:**
- SFML 2.5+ (Görselleştirme)
- CMake 3.16+
- C++17 uyumlu derleyici
- Google Test (testler için)

---

## SLİDE 17: GÖRSEL SONUÇLAR
**Başlık:** İşleme Sonuçları

**Örnek Akış:**
1. **Orijinal Görüntü** → `01_orijinal.pgm`
2. **Gaussian Blur** → `02_gaussian_blur.pgm`
3. **Edge Detection** → `03_kenar_tespiti.pgm`
4. **Brightness Adjust** → `04_parlaklik_ayarli.pgm`
5. **Pipeline Sonucu** → `05_pipeline_sonucu.pgm`

**Görsel Öneriler:**
- Her adımın önce/sonra karşılaştırması
- Pipeline'ın tüm adımlarını gösteren görsel
- Farklı parametrelerle sonuçlar

---

## SLİDE 18: PROJE YAPISI
**Başlık:** Proje Yapısı

**Açıklama Metni:**
Proje, standart C++ kütüphane yapısına uygun olarak organize edilmiştir. Header ve kaynak dosyalar ayrı dizinlerde tutularak modüler bir yapı oluşturulmuştur. Bu yapı, kodun bakımını kolaylaştırır ve yeni özelliklerin eklenmesini hızlandırır.

**Dizin Organizasyonu:**
- **include/**: Tüm sınıf tanımlamaları (header dosyaları) - diğer projeler tarafından kullanılabilir
- **src/**: Sınıf implementasyonları ve ana program dosyası
- **tests/**: Unit test ve entegrasyon test dosyaları
- **build/**: Derleme çıktıları ve oluşturulan kütüphane dosyaları

**Dosya Yapısı:**
```
ImageProcessingLib/
├── include/          # Header dosyaları
│   ├── Image.h
│   ├── Filter.h
│   ├── GaussianBlur.h
│   ├── EdgeDetection.h
│   ├── BrightnessAdjust.h
│   ├── FilterPipeline.h
│   └── ImageViewer.h
├── src/              # Kaynak kodlar
│   ├── Image.cpp
│   ├── GaussianBlur.cpp
│   ├── EdgeDetection.cpp
│   ├── BrightnessAdjust.cpp
│   ├── FilterPipeline.cpp
│   ├── ImageViewer.cpp
│   └── main.cpp
├── tests/            # Test dosyaları
│   ├── test_image.cpp
│   ├── test_filters.cpp
│   ├── test_pipeline.cpp
│   └── test_integration.cpp
├── build/            # Derleme çıktıları
├── CMakeLists.txt    # Build konfigürasyonu
└── README.md         # Dokümantasyon
```

**Avantajlar:**
- ✅ Ayrı header/source yapısı → Daha iyi derleme yönetimi
- ✅ Test dizini → Testlerin ayrı tutulması
- ✅ Modüler yapı → Kolay genişletilebilirlik
- ✅ Standart yapı → Diğer geliştiriciler için anlaşılabilir

---

## SLİDE 19: GENİŞLETME İMKANLARI
**Başlık:** Gelecek Geliştirmeler

**Yeni Filtreler:**
- 🔍 Sharpen (Keskinleştirme)
- 🎨 Contrast Adjust (Kontrast)
- 🎭 Sepia (Eski fotoğraf efekti)
- 🌈 Color Balance (Renk dengesi)
- 📐 Rotate/Flip (Döndürme/Çevirme)

**Yeni Özellikler:**
- 📁 Daha fazla format desteği (PNG, BMP)
- 🎬 Renkli görüntü desteği (RGB)
- ⚡ Performans iyileştirmeleri (multithreading)
- 🎛️ GUI arayüzü
- 📊 İstatistiksel analiz

---

## SLİDE 20: SONUÇ
**Başlık:** Proje Özeti ve Kazanımlar

**✅ Başarılar:**
- Modüler ve genişletilebilir kütüphane
- OOP prensiplerinin uygulanması
- Kapsamlı test altyapısı
- Temiz kod yapısı

**✅ Öğrenilenler:**
- C++ ile nesne yönelimli programlama
- Tasarım desenleri (Strategy, Composite)
- Görüntü işleme algoritmaları
- Test-driven development
- CMake build sistemi

**✅ Kullanım Alanları:**
- Görüntü ön işleme
- Bilgisayarlı görü
- Dijital sinyal işleme
- Eğitim projeleri

---

## SLİDE 21: SORULAR?
**Başlık:** Teşekkürler!
**Alt Başlık:** Sorularınız?

**İletişim:**
- GitHub: [repository link]
- Email: [email adresi]
- Dokümantasyon: README.md

---

## GÖRSEL ÖNERİLERİ

### Slayt 4 (Sınıflar Özeti) için:
- Sınıf listesi tablosu
- Her sınıf için ikon veya görsel

### Slayt 5 (Mimari) için:
- Sınıf diyagramı (UML stilinde)
- Ok işaretleri ile ilişkiler

### Slayt 7-9 (Filtreler) için:
- Her filtre için önce/sonra görseli
- Parametre değişikliklerinin etkisi

### Slayt 12 (Kod Örneği) için:
- Syntax highlighting ile kod
- Ekran görüntüsü

### Slayt 17 (Görsel Sonuçlar) için:
- 5 aşamalı pipeline görseli
- Yan yana karşılaştırma

### Slayt 20 (Sonuç) için:
- Proje ekran görüntüsü
- Test sonuçları

---

## SUNUM İPUÇLARI

1. **Slayt 1-3:** Hızlı geçiş (30 saniye)
2. **Slayt 4:** Sınıflar özeti (1 dakika)
3. **Slayt 5-11:** Her sınıf için 1-2 dakika
4. **Slayt 12:** Kod örneğini okuyup açıkla (2 dakika)
5. **Slayt 13-16:** Teknik detaylar (1-2 dakika)
6. **Slayt 17:** Görselleri göster (1-2 dakika)
7. **Slayt 18-20:** Özet ve sonuç (1 dakika)
8. **Slayt 21:** Soru-cevap

**Toplam Süre:** ~18-22 dakika (ayarlanabilir)

