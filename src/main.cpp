#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include "ImageViewer.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace GorselIsleme;

int main() { 
    ImageViewer viewer;
    std::unique_ptr<Image> gorsel;
    

    std::string dosya_adi;
    std::cout << "Gorsel dosya adini girin (ornek: resim.pgm veya resim.jpg): ";
    std::cin >> dosya_adi;
    
    std::string uzanti = dosya_adi.substr(dosya_adi.find_last_of('.') + 1);
    std::transform(uzanti.begin(), uzanti.end(), uzanti.begin(), ::tolower);
    
    if (uzanti == "jpg" || uzanti == "jpeg") {

        gorsel = Image::loadJPEG(dosya_adi);
    } else {
        gorsel = Image::load(dosya_adi);
       
    }

    if (!gorsel) {
        std::cout << "Dosya bulunamadi! Program sonlandiriliyor.\n";
        return 1;
    }
    
    
    viewer.goster("Orijinal", *gorsel);
    
    GaussianBlur bulanik_filtresi(10.0, 9);
    auto bulanik_sonuc = bulanik_filtresi.apply(*gorsel);
    viewer.goster("Bulanik", *bulanik_sonuc);

    EdgeDetection kenar_filtresi;
    auto kenar_sonuc = kenar_filtresi.apply(*bulanik_sonuc);
    viewer.goster("Kenar", *kenar_sonuc);

   BrightnessAdjust parlaklik_filtresi(0.1);
   auto parlak_sonuc = parlaklik_filtresi.apply(*gorsel);
   viewer.goster("Parlak", *parlak_sonuc);

    
    std::cout << "Tamamlandi! Cikmak icin bir tusa basin...\n";
    viewer.waitKey(0);
    
    return 0;
}