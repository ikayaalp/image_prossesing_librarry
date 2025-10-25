#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include "ImageViewer.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace ImageProcessing;

int main() {
    std::cout << "=== Modern C++ Görüntü İşleme Kütüphanesi Demo ===\n\n";
    
    // Görüntü görüntüleyici oluştur
    ImageViewer viewer;
    
    try {
        // Kullanıcının görüntüsünü yükle veya test görüntüsü oluştur
        std::cout << "1. Görüntü yükleniyor...\n";
        std::unique_ptr<Image> my_image;
        
        // Önce JPEG dosyasını yüklemeyi dene
        my_image = Image::loadJPEG("vesikalık.jpeg");
        if (!my_image) {
            std::cout << "   ⚠️ JPEG dosyası bulunamadı, test görüntüsü oluşturuluyor...\n";
            // Test görüntüsü oluştur
            my_image = std::make_unique<Image>(300, 300, 1);
            
            // Dairesel pattern oluştur
            for (int y = 0; y < 300; ++y) {
                for (int x = 0; x < 300; ++x) {
                    int center_x = 150, center_y = 150;
                    double distance = std::sqrt((x - center_x) * (x - center_x) + 
                                               (y - center_y) * (y - center_y));
                    
                    if (distance < 80) {
                        my_image->at(x, y) = 255; // Beyaz daire
                    } else if (distance < 120) {
                        my_image->at(x, y) = 128; // Gri halka
                    } else {
                        my_image->at(x, y) = 0; // Siyah arka plan
                    }
                }
            }
        }
        
        std::cout << "   ✓ Görüntü yüklendi (" 
                  << my_image->getWidth() << "x" << my_image->getHeight() << ")\n";
        
        // Orijinal görüntüyü göster
        viewer.imshow("Orijinal Görüntü", *my_image);
        std::cout << "   ✓ Orijinal görüntü pencerede gösterildi\n";
        
        // Filtre sonuçlarını saklamak için değişkenler
        std::unique_ptr<Image> blurred_image;
        std::unique_ptr<Image> edge_image;
        std::unique_ptr<Image> bright_image;
        std::unique_ptr<Image> pipeline_result;
        
        std::cout << "\n2. Filtreler uygulanıyor...\n";
        
        // 1. Gaussian Blur uygula
        std::cout << "   🔹 Gaussian Blur filtresi uygulanıyor...\n";
        auto blur_filter = std::make_unique<GaussianBlur>(2.0, 5);
        blurred_image = blur_filter->apply(*my_image);
        viewer.imshow("Gaussian Blur", *blurred_image);
        std::cout << "   ✓ Bulanıklaştırma tamamlandı\n";
        
        // 2. Kenar tespiti uygula
        std::cout << "   🔹 Kenar tespiti uygulanıyor...\n";
        auto edge_filter = std::make_unique<EdgeDetection>();
        edge_image = edge_filter->apply(*my_image);
        viewer.imshow("Kenar Tespiti", *edge_image);
        std::cout << "   ✓ Kenar tespiti tamamlandı\n";
        
        // 3. Parlaklık ayarlama uygula
        std::cout << "   🔹 Parlaklık ayarlama uygulanıyor...\n";
        auto brightness_filter = std::make_unique<BrightnessAdjust>(1.3);
        bright_image = brightness_filter->apply(*my_image);
        viewer.imshow("Parlaklık Artırılmış", *bright_image);
        std::cout << "   ✓ Parlaklık ayarlama tamamlandı\n";
        
        // 4. Filtre pipeline'ı oluştur ve uygula
        std::cout << "\n3. Filtre pipeline'ı oluşturuluyor...\n";
        FilterPipeline pipeline;
        
        // Pipeline'a filtreler ekle
        pipeline.addFilter(std::make_unique<GaussianBlur>(1.5, 3));
        pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
        pipeline.addFilter(std::make_unique<EdgeDetection>());
        
        std::cout << "   🔹 Pipeline uygulanıyor (Blur → Brightness → Edge Detection)...\n";
        pipeline_result = pipeline.apply(*my_image);
        viewer.imshow("Pipeline Sonucu", *pipeline_result);
        std::cout << "   ✓ Pipeline tamamlandı\n";
        
        // Pipeline'daki filtreleri göster
        std::cout << "\n4. Pipeline bilgileri:\n";
        auto filter_names = pipeline.getAllFilterNames();
        for (size_t i = 0; i < filter_names.size(); ++i) {
            std::cout << "   " << (i+1) << ". " << filter_names[i] << "\n";
        }
        
        // Görüntü istatistikleri
        std::cout << "\n5. Görüntü istatistikleri:\n";
        std::cout << "   📊 Orijinal görüntü:\n";
        std::cout << "      - Boyut: " << my_image->getWidth() << "x" << my_image->getHeight() << "\n";
        std::cout << "      - Ortalama: " << my_image->getMeanValue() << "\n";
        std::cout << "      - Min: " << my_image->getMinValue().value_or(0) << "\n";
        std::cout << "      - Max: " << my_image->getMaxValue().value_or(0) << "\n";
        
        std::cout << "\n   📊 Pipeline sonucu:\n";
        std::cout << "      - Ortalama: " << pipeline_result->getMeanValue() << "\n";
        std::cout << "      - Min: " << pipeline_result->getMinValue().value_or(0) << "\n";
        std::cout << "      - Max: " << pipeline_result->getMaxValue().value_or(0) << "\n";
        
        // Sonuçları kaydet
        std::cout << "\n6. Sonuçlar kaydediliyor...\n";
        my_image->save("01_orijinal.pgm");
        blurred_image->save("02_gaussian_blur.pgm");
        edge_image->save("03_kenar_tespiti.pgm");
        bright_image->save("04_parlaklik_ayarli.pgm");
        pipeline_result->save("05_pipeline_sonucu.pgm");
        
        std::cout << "   ✓ Tüm sonuçlar PGM formatında kaydedildi\n";
        
        std::cout << "\n🎉 Demo tamamlandı! Pencereleri kapatmak için herhangi bir tuşa basın...\n";
        
        // Kullanıcı girişini bekle
        viewer.waitKey(0);
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Hata: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}