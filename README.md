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

### Ana Demo
```bash
./image_demo
```

### Sadece Kenar Bulma
```bash
./kenar_bulma
```

## Ornek Kod

```cpp
#include "Image.h"
#include "EdgeDetection.h"
#include "ImageViewer.h"

using namespace ImageProcessing;

int main() {
    // Gorsel yukle
    auto gorsel = Image::load("resim.pgm");
    
    // Kenar bul
    EdgeDetection kenar_filtresi;
    auto sonuc = kenar_filtresi.apply(*gorsel);
    
    // Goster
    ImageViewer viewer;
    viewer.imshow("Kenar", *sonuc);
    viewer.waitKey(0);
    
    return 0;
}
```

## Ozellikler

- Gorsel yukleme ve kaydetme
- Kenar bulma
- Bulaniklastirma
- Parlaklik ayari
- Filtre pipeline'i
- Gorsel gosterme

## Dosyalar

- `src/main.cpp` - Ana demo programi
- `src/kenar_bulma.cpp` - Sadece kenar bulma
- `include/` - Kutuphane baslik dosyalari
- `src/` - Kutuphane kaynak dosyalari