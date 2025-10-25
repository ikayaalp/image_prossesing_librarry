#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include "ImageViewer.h"
#include <iostream>

using namespace GorselIsleme;

int main() {
    std::cout << "Gorsel Isleme Kutuphanesi Demo\n";
    std::cout << "==============================\n\n";
    
    ImageViewer viewer;
    
    // Test gorseli olustur
    std::cout << "Test gorseli olusturuluyor...\n";
    Image test_gorsel(200, 200, 1);
    
    // Basit pattern
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 200; ++x) {
            if (x > 50 && x < 150 && y > 50 && y < 150) {
                test_gorsel.at(x, y) = 255;
            } else {
                test_gorsel.at(x, y) = 0;
            }
        }
    }
    
    std::cout << "Gorsel olusturuldu: " << test_gorsel.getWidth() << "x" << test_gorsel.getHeight() << "\n";
    
    // Orijinal goster
    viewer.imshow("Orijinal", test_gorsel);
    
    // Bulaniklastirma
    std::cout << "Bulaniklastirma uygulaniyor...\n";
    GaussianBlur bulanik_filtresi(2.0, 5);
    auto bulanik_sonuc = bulanik_filtresi.apply(test_gorsel);
    viewer.imshow("Bulanik", *bulanik_sonuc);
    
    // Kenar bulma
    std::cout << "Kenar bulunuyor...\n";
    EdgeDetection kenar_filtresi;
    auto kenar_sonuc = kenar_filtresi.apply(test_gorsel);
    viewer.imshow("Kenar", *kenar_sonuc);
    
    // Parlaklik ayari
    std::cout << "Parlaklik ayarlaniyor...\n";
    BrightnessAdjust parlaklik_filtresi(1.5);
    auto parlak_sonuc = parlaklik_filtresi.apply(test_gorsel);
    viewer.imshow("Parlak", *parlak_sonuc);
    
    // Pipeline
    std::cout << "Pipeline uygulaniyor...\n";
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto pipeline_sonuc = pipeline.apply(test_gorsel);
    viewer.imshow("Pipeline Sonucu", *pipeline_sonuc);
    
    // Kaydet
    test_gorsel.save("orijinal.pgm");
    bulanik_sonuc->save("bulanik.pgm");
    kenar_sonuc->save("kenar.pgm");
    parlak_sonuc->save("parlak.pgm");
    pipeline_sonuc->save("pipeline.pgm");
    
    std::cout << "Sonuclar kaydedildi!\n";
    std::cout << "Tamamlandi! Cikmak icin bir tusa basin...\n";
    
    viewer.waitKey(0);
    
    return 0;
}