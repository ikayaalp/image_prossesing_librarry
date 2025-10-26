#include "Image.h"
#include "EdgeDetection.h"
#include "ImageViewer.h"
#include <iostream>

using namespace GorselIsleme;

int main() {
    std::cout << "Kenar Bulma Programi\n";
    std::cout << "===================\n\n";
    
    ImageViewer viewer;
    
    // Dosya adi al
    std::string dosya;
    std::cout << "Dosya adini girin: ";
    std::cin >> dosya;
    
    // Gorsel yukle
    std::cout << "Gorsel yukleniyor...\n";
    auto gorsel = Image::load(dosya);
    
    if (!gorsel) {
        std::cout << "Dosya bulunamadi!\n";
        return 1;
    }
    
    std::cout << "Gorsel yuklendi: " << gorsel->getWidth() << "x" << gorsel->getHeight() << "\n";
    
    // Orijinal goster
    viewer.goster("Orijinal", *gorsel);
    
    // Kenar bul
    std::cout << "Kenar bulunuyor...\n";
    EdgeDetection kenar_filtresi;
    auto kenar_sonucu = kenar_filtresi.apply(*gorsel);
    
    // Kenar sonucunu goster
    viewer.goster("Kenar Sonucu", *kenar_sonucu);
    
    // Kaydet
    kenar_sonucu->save("kenar.pgm");
    std::cout << "Sonuc kaydedildi: kenar.pgm\n";
    
    std::cout << "Tamamlandi! Cikmak icin bir tusa basin...\n";
    viewer.waitKey(0);
    
    return 0;
}