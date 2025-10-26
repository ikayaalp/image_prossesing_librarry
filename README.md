# Gorsel Isleme Kutuphanesi

Basit C++ gorsel isleme kutuphanesi.

## Kurulum

```bash
git clone https://github.com/ikayaalp/image_prossesing_librarry.git
cd image_prossesing_librarry
mkdir build && cd build
cmake ..
make
```

## Kullanim

```bash
./image_demo
```

## Yapilan Islemler

### 1. Gorsel Yukleme
- PGM dosyalari yuklenir
- JPEG dosyalari PGM'e donusturulur

### 2. Bulaniklastirma (Gaussian Blur)
- Gorseli bulaniklastirir
- Parametreler: Sigma (bulaniklik), Kernel boyutu

### 3. Kenar Tespiti (Edge Detection)
- Gorseldeki kenarlari bulur
- Sobel operatörü kullanir

### 4. Parlaklik Ayari (Brightness Adjust)
- Gorselin parlakligini degistirir
- Parametre: Parlaklik faktoru

### 5. Filtre Pipeline
- Birden fazla filtreyi sirasiyla uygular
- Zincirleme filtre islemi

### 6. Gorsel Gosterme
- SFML ile pencere acar
- Gorseli ekranda gosterir

### 7. Dosya Kaydetme
- Islenmis gorselleri PGM formatinda kaydeder

## Ornek Kod

```cpp
#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "ImageViewer.h"

using namespace GorselIsleme;

int main() {
    // Gorsel yukle
    auto gorsel = Image::load("resim.pgm");
    
    // Bulaniklastir
    GaussianBlur blur(2.0, 5);
    auto bulanik = blur.apply(*gorsel);
    
    // Kenar bul
    EdgeDetection kenar;
    auto kenar_sonuc = kenar.apply(*gorsel);
    
    // Goster
    ImageViewer viewer;
    viewer.goster("Sonuc", *kenar_sonuc);
    viewer.waitKey(0);
    
    return 0;
}
```

## Dosyalar

- `src/main.cpp` - Ana program
- `include/` - Kutuphane baslik dosyalari
- `src/` - Kutuphane kaynak dosyalari
- `tests/` - Test dosyalari