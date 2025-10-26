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
    
    // Kullanıcıdan dosya adı al
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
    
    
    // Orijinal goster
    viewer.goster("Orijinal", *gorsel);
    
    // Bulaniklastirma
    std::cout << "Bulaniklastirma uygulaniyor...\n";
    GaussianBlur bulanik_filtresi(2.0, 5);
    auto bulanik_sonuc = bulanik_filtresi.apply(*gorsel);
    viewer.goster("Bulanik", *bulanik_sonuc);
    
    // Kenar bulma
    std::cout << "Kenar bulunuyor...\n";
    EdgeDetection kenar_filtresi;
    auto kenar_sonuc = kenar_filtresi.apply(*gorsel);
    viewer.goster("Kenar", *kenar_sonuc);
    
    // Parlaklik ayari
    std::cout << "Parlaklik ayarlaniyor...\n";
    BrightnessAdjust parlaklik_filtresi(1.5);
    auto parlak_sonuc = parlaklik_filtresi.apply(*gorsel);
    viewer.goster("Parlak", *parlak_sonuc);
    
    // Pipeline
    std::cout << "Pipeline uygulaniyor...\n";
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto pipeline_sonuc = pipeline.apply(*gorsel);
    viewer.goster("Pipeline Sonucu", *pipeline_sonuc);
    
    // Kaydet
    std::cout << "Sonuclar kaydediliyor...\n";
    gorsel->save("orijinal.pgm");
    bulanik_sonuc->save("bulanik.pgm");
    kenar_sonuc->save("kenar.pgm");
    parlak_sonuc->save("parlak.pgm");
    pipeline_sonuc->save("pipeline.pgm");
    
    std::cout << "Sonuclar kaydedildi!\n";
    std::cout << "- orijinal.pgm\n";
    std::cout << "- bulanik.pgm\n";
    std::cout << "- kenar.pgm\n";
    std::cout << "- parlak.pgm\n";
    std::cout << "- pipeline.pgm\n";
    
    std::cout << "Tamamlandi! Cikmak icin bir tusa basin...\n";
    viewer.waitKey(0);
    
    return 0;
}