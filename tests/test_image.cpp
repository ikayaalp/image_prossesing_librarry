#include <gtest/gtest.h>
#include "Image.h"

using namespace ImageProcessing;

class ImageTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_image = std::make_unique<Image>(10, 10, 1);
        
        // Create test pattern
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                test_image->at(x, y) = static_cast<Image::Pixel>(x + y);
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(ImageTest, ConstructorTest) {
    Image image(5, 5, 1);
    EXPECT_EQ(image.getWidth(), 5);
    EXPECT_EQ(image.getHeight(), 5);
    EXPECT_EQ(image.getChannels(), 1);
    EXPECT_FALSE(image.empty());
}

TEST_F(ImageTest, InvalidConstructorTest) {
    EXPECT_THROW(Image(-1, 5, 1), std::invalid_argument);
    EXPECT_THROW(Image(5, -1, 1), std::invalid_argument);
    EXPECT_THROW(Image(5, 5, -1), std::invalid_argument);
}

TEST_F(ImageTest, PixelAccessTest) {
    test_image->at(0, 0) = 255;
    EXPECT_EQ(test_image->at(0, 0), 255);
    
    test_image->operator()(1, 1) = 128;
    EXPECT_EQ(test_image->operator()(1, 1), 128);
}

TEST_F(ImageTest, InvalidPixelAccessTest) {
    EXPECT_THROW(test_image->at(-1, 0), std::out_of_range);
    EXPECT_THROW(test_image->at(0, -1), std::out_of_range);
    EXPECT_THROW(test_image->at(10, 0), std::out_of_range);
    EXPECT_THROW(test_image->at(0, 10), std::out_of_range);
}

TEST_F(ImageTest, CopyConstructorTest) {
    Image copy(*test_image);
    EXPECT_EQ(copy.getWidth(), test_image->getWidth());
    EXPECT_EQ(copy.getHeight(), test_image->getHeight());
    EXPECT_EQ(copy.at(0, 0), test_image->at(0, 0));
}

TEST_F(ImageTest, MoveConstructorTest) {
    Image original(5, 5, 1);
    original.at(0, 0) = 100;
    
    Image moved(std::move(original));
    EXPECT_EQ(moved.getWidth(), 5);
    EXPECT_EQ(moved.at(0, 0), 100);
    EXPECT_EQ(original.getWidth(), 0); // Moved from
}

TEST_F(ImageTest, AssignmentOperatorTest) {
    Image other(5, 5, 1);
    other.at(0, 0) = 200;
    
    *test_image = other;
    EXPECT_EQ(test_image->getWidth(), 5);
    EXPECT_EQ(test_image->at(0, 0), 200);
}

TEST_F(ImageTest, ResizeTest) {
    test_image->resize(15, 15);
    EXPECT_EQ(test_image->getWidth(), 15);
    EXPECT_EQ(test_image->getHeight(), 15);
}

TEST_F(ImageTest, InvalidResizeTest) {
    EXPECT_THROW(test_image->resize(-1, 5), std::invalid_argument);
    EXPECT_THROW(test_image->resize(5, -1), std::invalid_argument);
}

TEST_F(ImageTest, ClearTest) {
    test_image->clear();
    EXPECT_EQ(test_image->at(0, 0), 0);
    EXPECT_EQ(test_image->at(5, 5), 0);
}

TEST_F(ImageTest, StatisticsTest) {
    auto min_val = test_image->getMinValue();
    auto max_val = test_image->getMaxValue();
    
    EXPECT_TRUE(min_val.has_value());
    EXPECT_TRUE(max_val.has_value());
    EXPECT_EQ(min_val.value(), 0);
    EXPECT_EQ(max_val.value(), 18);
    
    double mean = test_image->getMeanValue();
    EXPECT_GT(mean, 0);
    EXPECT_LT(mean, 255);
}

TEST_F(ImageTest, EmptyImageTest) {
    Image empty_image;
    EXPECT_TRUE(empty_image.empty());
    EXPECT_EQ(empty_image.getTotalPixels(), 0);
    
    auto min_val = empty_image.getMinValue();
    auto max_val = empty_image.getMaxValue();
    EXPECT_FALSE(min_val.has_value());
    EXPECT_FALSE(max_val.has_value());
}

TEST_F(ImageTest, PixelSearchTest) {
    auto pixel = test_image->getPixel(0, 0);
    EXPECT_TRUE(pixel.has_value());
    EXPECT_EQ(pixel.value(), 0);
    
    auto invalid_pixel = test_image->getPixel(-1, 0);
    EXPECT_FALSE(invalid_pixel.has_value());
}

TEST_F(ImageTest, PixelSetTest) {
    test_image->setPixel(5, 5, 0, 100);
    EXPECT_EQ(test_image->at(5, 5), 100);
    
    // Invalid coordinates - should not throw
    test_image->setPixel(-1, 0, 0, 100);
    // Should not change anything
}

TEST_F(ImageTest, NormalizeTest) {
    // Set all pixels to same value
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            test_image->at(x, y) = 50;
        }
    }
    
    test_image->normalize();
    // Normalize should not change values when all pixels are same
    EXPECT_EQ(test_image->at(0, 0), 50);
}

TEST_F(ImageTest, HistogramTest) {
    std::vector<int> histogram;
    test_image->histogram(histogram);
    
    EXPECT_EQ(histogram.size(), 256);
    
    // Histogram sum should equal total pixels
    int sum = std::accumulate(histogram.begin(), histogram.end(), 0);
    EXPECT_EQ(sum, test_image->getTotalPixels());
}

TEST_F(ImageTest, ApplyFunctionTest) {
    auto original_value = test_image->at(0, 0);
    
    // Apply function that doubles the value
    test_image->applyFunction([](Image::Pixel p) -> Image::Pixel {
        return static_cast<Image::Pixel>(std::min(255, static_cast<int>(p * 2)));
    });
    
    EXPECT_GE(test_image->at(0, 0), original_value);
}

TEST_F(ImageTest, ApplyKernelTest) {
    // Simple kernel
    std::vector<std::vector<double>> kernel = {
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0}
    };
    
    auto original_value = test_image->at(1, 1);
    test_image->applyKernel(kernel);
    
    // Identity kernel should not change values much
    EXPECT_NEAR(test_image->at(1, 1), original_value, 1);
}
