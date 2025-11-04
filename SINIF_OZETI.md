# ImageProcessingLib - Sınıf Özeti

## 📸 **Image** (Temel Görüntü Sınıfı)
**Ne işe yarar?** Görüntü verilerini saklar ve yönetir.

**Temel özellikler:**
- Piksel verilerini `unsigned char` vektöründe tutar
- Genişlik, yükseklik, kanal sayısı bilgilerini saklar
- PGM formatında dosya okuma/yazma (`load()`, `save()`)
- JPEG dosyalarını okuma (`loadJPEG()`)
- Güvenli piksel erişimi (`at()` metodu ile sınır kontrolü)
- Kopyalama ve atama operatörleri

**Kullanım:** Tüm görüntü işlemlerinin temel veri yapısı. Diğer sınıflar bu sınıfı kullanır.

---

## 🔧 **Filter** (Soyut Arayüz Sınıfı)
**Ne işe yarar?** Tüm filtrelerin ortak arayüzünü tanımlar (Strategy Pattern).

**Temel özellikler:**
- Saf sanal (pure virtual) fonksiyonlar:
  - `apply()`: Görüntüye filtre uygular
  - `getName()`: Filtre adını döndürür
  - `clone()`: Filtrenin kopyasını oluşturur

**Kullanım:** Tüm filtreler bu arayüzü implement eder. Böylece farklı filtreler aynı şekilde kullanılabilir.

---

## 🌫️ **GaussianBlur** (Bulanıklaştırma Filtresi)
**Ne işe yarar?** Görüntüyü yumuşatır ve gürültüyü azaltır.

**Temel özellikler:**
- `Filter` sınıfından türetilmiştir
- Parametreler:
  - `sigma`: Bulanıklık yoğunluğu (varsayılan: 1.0)
  - `kernel_size`: Kernel boyutu (varsayılan: 3)
- Gaussian fonksiyonuna göre kernel oluşturur
- Konvolüsyon işlemi ile görüntüyü işler

**Kullanım:** Gürültü azaltma, yumuşatma efektleri için.

---

## 🔍 **EdgeDetection** (Kenar Tespiti Filtresi)
**Ne işe yarar?** Görüntüdeki kenarları (edges) tespit eder ve vurgular.

**Temel özellikler:**
- `Filter` sınıfından türetilmiştir
- Sobel operatörü kullanır (yatay ve dikey gradyanlar)
- Yön seçimi:
  - `Horizontal`: Sadece yatay kenarlar
  - `Vertical`: Sadece dikey kenarlar
  - `Both`: Tüm kenarlar (varsayılan)
- Konvolüsyon işlemi ile kenarları tespit eder

**Kullanım:** Nesne tespiti, görüntü analizi, özellik çıkarımı için.

---

## 💡 **BrightnessAdjust** (Parlaklık Ayarı Filtresi)
**Ne işe yarar?** Görüntünün parlaklığını artırır veya azaltır.

**Temel özellikler:**
- `Filter` sınıfından türetilmiştir
- Parametre:
  - `brightness_factor`: Parlaklık çarpanı (1.0 = değişmez, <1.0 = koyulaştır, >1.0 = aydınlat)
- Her pikselin değerini çarpan ile çarpar (sınır kontrolü ile)

**Kullanım:** Görüntü parlaklık düzeltmeleri, kontrast ayarları için.

---

## 🔄 **FilterPipeline** (Filtre Boru Hattı)
**Ne işe yarar?** Birden fazla filtreyi sırayla uygular (Composite Pattern).

**Temel özellikler:**
- Filtreleri bir vektörde saklar
- `addFilter()`: Boru hattına filtre ekler
- `removeFilter()`: Belirli bir filtreyi kaldırır
- `apply()`: Tüm filtreleri sırayla uygular (ilk filtre çıktısı ikinci filtreye giriş olur)
- `getAllFilterNames()`: Tüm filtre isimlerini listeler
- `clone()`: Boru hattının kopyasını oluşturur

**Kullanım:** Birden fazla işlemi tek seferde uygulamak için. Örnek: Blur → Edge Detection → Brightness Adjust

---

## 🖼️ **ImageViewer** (Görüntü Görüntüleyici)
**Ne işe yarar?** Görüntüleri ekranda gösterir (SFML kütüphanesi kullanır).

**Temel özellikler:**
- `goster()`: Görüntüyü belirtilen pencere adıyla gösterir
- `waitKey()`: Kullanıcı girişi bekler
- `destroyWindow()`: Belirli bir pencereyi kapatır
- `destroyAllWindows()`: Tüm pencereleri kapatır
- `isWindowOpen()`: Pencere açık mı kontrol eder

**Kullanım:** İşlenmiş görüntüleri kullanıcıya göstermek için.

---

## 📊 **Sınıf İlişkileri**

```
Image (temel veri yapısı)
    ↑
    ├── Filter (soyut arayüz)
    │   ├── GaussianBlur
    │   ├── EdgeDetection
    │   └── BrightnessAdjust
    │
    ├── FilterPipeline (filtreleri yönetir)
    │   └── Filter* (filtrelere referans tutar)
    │
    └── ImageViewer (görüntüleri gösterir)
```

---

## 🎯 **Tasarım Desenleri**

1. **Strategy Pattern**: `Filter` arayüzü ve somut filtreler
2. **Composite Pattern**: `FilterPipeline` birden fazla filtreyi birleştirir
3. **Factory Pattern**: `Image::load()` ve `Image::loadJPEG()` statik metodları

---

## 💻 **Kullanım Örneği (main.cpp)**

```cpp
// Görüntü yükle
auto gorsel = Image::load("resim.pgm");

// Filtreler oluştur
GaussianBlur bulanik(10.0, 9);
EdgeDetection kenar;
BrightnessAdjust parlaklik(0.1);

// Filtreleri uygula
auto bulanik_sonuc = bulanik.apply(*gorsel);
auto kenar_sonuc = kenar.apply(*bulanik_sonuc);
auto parlak_sonuc = parlaklik.apply(*gorsel);

// Sonuçları göster
ImageViewer viewer;
viewer.goster("Orijinal", *gorsel);
viewer.goster("Bulanik", *bulanik_sonuc);
viewer.goster("Kenar", *kenar_sonuc);
viewer.goster("Parlak", *parlak_sonuc);
```

