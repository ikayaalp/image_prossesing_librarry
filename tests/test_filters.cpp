#include <gtest/gtest.h>
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "Image.h"

using namespace GorselIsleme;

class FilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_image = std::make_unique<Image>(50, 50, 1);
        
        // Basit pattern oluştur
        for (int y = 0; y < 50; ++y) {
            for (int x = 0; x < 50; ++x) {
                if (x > 20 && x < 30 && y > 20 && y < 30) {
                    test_image->at(x, y) = 255;
                } else {
                    test_image->at(x, y) = 0;
                }
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(FilterTest, GaussianBlurTest) {
    GaussianBlur blur_filter(1.0, 3);
    auto result = blur_filter.apply(*test_image);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
    EXPECT_EQ(result->getChannels(), test_image->getChannels());
}

TEST_F(FilterTest, EdgeDetectionTest) {
    EdgeDetection edge_filter;
    auto result = edge_filter.apply(*test_image);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(FilterTest, BrightnessAdjustTest) {
    BrightnessAdjust bright_filter(1.5);
    auto result = bright_filter.apply(*test_image);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(FilterTest, FilterNamesTest) {
    GaussianBlur blur_filter;
    EdgeDetection edge_filter;
    BrightnessAdjust bright_filter;
    
    EXPECT_EQ(blur_filter.getName(), "GaussianBlur");
    EXPECT_EQ(edge_filter.getName(), "EdgeDetection");
    EXPECT_EQ(bright_filter.getName(), "BrightnessAdjust");
}

TEST_F(FilterTest, FilterCloneTest) {
    GaussianBlur original_filter(2.0, 5);
    auto cloned_filter = original_filter.clone();
    
    ASSERT_NE(cloned_filter, nullptr);
    EXPECT_EQ(cloned_filter->getName(), "GaussianBlur");
}