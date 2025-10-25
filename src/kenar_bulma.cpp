#include "Image.h"
#include "EdgeDetection.h"
#include "ImageViewer.h"
#include <iostream>
#include <memory>
#include <string>

using namespace ImageProcessing;

int main() {
    std::cout << "=== Sadece Kenar Bulma Programı ===\n\n";
    
    // Görüntü görüntüleyici oluştur
    ImageViewer viewer;
    
    // Kullanıcıdan dosya adı al
    std::string dosya_adi;
    std::cout << "Görüntü dosyası adını girin (örn: resim.pgm): ";
    std::cin >> dosya_adi;
    
    // Görüntüyü yükle
    std::cout << "1. Görüntü yükleniyor: " << dosya_adi << "\n";
    auto my_image = Image::load(dosya_adi);
    
    if (!my_image) {
        std::cout << "   ❌ Dosya bulunamadı! Dosya adını kontrol edin.\n";
        return 1;
    }
    
    std::cout << "   ✓ Görüntü yüklendi (" 
              << my_image->getWidth() << "x" << my_image->getHeight() << ")\n";
    
    // Orijinal görüntüyü göster
    viewer.imshow("Orijinal Görüntü", *my_image);
    std::cout << "   ✓ Orijinal görüntü gösterildi\n";
    
    // KENAR BULMA
    std::cout << "\n2. Kenar tespiti yapılıyor...\n";
    auto edge_filter = std::make_unique<EdgeDetection>();
    auto kenar_sonucu = edge_filter->apply(*my_image);
    
    // Kenar sonucunu göster
    viewer.imshow("Kenar Tespiti Sonucu", *kenar_sonucu);
    std::cout << "   ✓ Kenar tespiti tamamlandı\n";
    
    // Sonucu kaydet
    kenar_sonucu->save("kenar_sonucu.pgm");
    std::cout << "   ✓ Sonuç 'kenar_sonucu.pgm' olarak kaydedildi\n";
    
    std::cout << "\n🎉 Kenar bulma tamamlandı! Pencereleri kapatmak için herhangi bir tuşa basın...\n";
    
    // Kullanıcı girişini bekle
    viewer.waitKey(0);
    
    return 0;
}