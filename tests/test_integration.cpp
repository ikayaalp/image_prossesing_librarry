#include <gtest/gtest.h>
#include "Image.h"
#include "FilterPipeline.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"

using namespace GorselIsleme;

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Daha büyük test görüntüsü
        test_image = std::make_unique<Image>(100, 100, 1);
        
        // Karmaşık pattern
        for (int y = 0; y < 100; ++y) {
            for (int x = 0; x < 100; ++x) {
                if ((x - 50) * (x - 50) + (y - 50) * (y - 50) < 400) {
                    test_image->at(x, y) = 255;
                } else if (x > 20 && x < 80 && y > 20 && y < 80) {
                    test_image->at(x, y) = 128;
                } else {
                    test_image->at(x, y) = 0;
                }
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(IntegrationTest, CompletePipelineTest) {
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.5, 5));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.3));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto result = pipeline.apply(*test_image);
    ASSERT_NE(result, nullptr);
    
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
    EXPECT_EQ(result->getChannels(), test_image->getChannels());
}

TEST_F(IntegrationTest, SequentialFilterTest) {
    // Gaussian Blur
    GaussianBlur blur_filter(1.0, 3);
    auto blurred = blur_filter.apply(*test_image);
    ASSERT_NE(blurred, nullptr);
    
    // Brightness Adjust
    BrightnessAdjust bright_filter(1.2);
    auto brightened = bright_filter.apply(*blurred);
    ASSERT_NE(brightened, nullptr);
    
    // Edge Detection
    EdgeDetection edge_filter;
    auto edges = edge_filter.apply(*brightened);
    ASSERT_NE(edges, nullptr);
    
    EXPECT_EQ(edges->getWidth(), test_image->getWidth());
    EXPECT_EQ(edges->getHeight(), test_image->getHeight());
}

TEST_F(IntegrationTest, SaveLoadIntegrationTest) {
    // Görüntüyü kaydet
    EXPECT_TRUE(test_image->save("integration_test.pgm"));
    
    // Görüntüyü yükle
    auto loaded_image = Image::load("integration_test.pgm");
    ASSERT_NE(loaded_image, nullptr);
    
    EXPECT_EQ(loaded_image->getWidth(), test_image->getWidth());
    EXPECT_EQ(loaded_image->getHeight(), test_image->getHeight());
    
    // Pipeline uygula
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto result = pipeline.apply(*loaded_image);
    ASSERT_NE(result, nullptr);
    
    // Sonucu kaydet
    EXPECT_TRUE(result->save("integration_result.pgm"));
}

TEST_F(IntegrationTest, PerformanceTest) {
    // Büyük görüntü ile performans testi
    Image large_image(200, 200, 1);
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 200; ++x) {
            large_image.at(x, y) = (x + y) % 256;
        }
    }
    
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(2.0, 7));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.1));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto result = pipeline.apply(large_image);
    ASSERT_NE(result, nullptr);
    
    EXPECT_EQ(result->getWidth(), 200);
    EXPECT_EQ(result->getHeight(), 200);
}