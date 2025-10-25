#include <gtest/gtest.h>
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"

using namespace ImageProcessing;

class FilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_image = std::make_unique<Image>(20, 20, 1);
        
        // Create test pattern
        for (int y = 0; y < 20; ++y) {
            for (int x = 0; x < 20; ++x) {
                if (x == 10 && y == 10) {
                    test_image->at(x, y) = 255; // Center pixel white
                } else {
                    test_image->at(x, y) = 0; // Others black
                }
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(FilterTest, GaussianBlurConstructorTest) {
    GaussianBlur filter(1.0, 5);
    EXPECT_EQ(filter.getName(), "GaussianBlur");
    EXPECT_EQ(filter.getSigma(), 1.0);
    EXPECT_EQ(filter.getKernelSize(), 5);
}

TEST_F(FilterTest, GaussianBlurInvalidConstructorTest) {
    EXPECT_THROW(GaussianBlur(-1.0, 5), std::invalid_argument);
    EXPECT_THROW(GaussianBlur(1.0, 4), std::invalid_argument);
    EXPECT_THROW(GaussianBlur(1.0, 0), std::invalid_argument);
}

TEST_F(FilterTest, GaussianBlurApplyTest) {
    GaussianBlur filter(1.0, 5);
    auto result = filter.apply(*test_image);
    
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
    EXPECT_EQ(result->getChannels(), test_image->getChannels());
}

TEST_F(FilterTest, GaussianBlurParameterTest) {
    GaussianBlur filter(1.0, 5);
    
    filter.setSigma(2.0);
    EXPECT_EQ(filter.getSigma(), 2.0);
    
    filter.setKernelSize(7);
    EXPECT_EQ(filter.getKernelSize(), 7);
}

TEST_F(FilterTest, GaussianBlurInvalidParameterTest) {
    GaussianBlur filter(1.0, 5);
    
    EXPECT_THROW(filter.setSigma(-1.0), std::invalid_argument);
    EXPECT_THROW(filter.setKernelSize(4), std::invalid_argument);
    EXPECT_THROW(filter.setKernelSize(0), std::invalid_argument);
}

TEST_F(FilterTest, GaussianBlurCloneTest) {
    GaussianBlur original(2.0, 7);
    auto cloned = original.clone();
    
    EXPECT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->getName(), "GaussianBlur");
}

TEST_F(FilterTest, EdgeDetectionConstructorTest) {
    EdgeDetection filter(EdgeDetection::Direction::Both);
    EXPECT_EQ(filter.getName(), "EdgeDetection");
    EXPECT_EQ(filter.getDirection(), EdgeDetection::Direction::Both);
}

TEST_F(FilterTest, EdgeDetectionApplyTest) {
    EdgeDetection filter(EdgeDetection::Direction::Both);
    auto result = filter.apply(*test_image);
    
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(FilterTest, EdgeDetectionDirectionTest) {
    EdgeDetection filter(EdgeDetection::Direction::Horizontal);
    EXPECT_EQ(filter.getDirection(), EdgeDetection::Direction::Horizontal);
    
    filter.setDirection(EdgeDetection::Direction::Vertical);
    EXPECT_EQ(filter.getDirection(), EdgeDetection::Direction::Vertical);
}

TEST_F(FilterTest, EdgeDetectionCloneTest) {
    EdgeDetection original(EdgeDetection::Direction::Both);
    auto cloned = original.clone();
    
    EXPECT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->getName(), "EdgeDetection");
}

TEST_F(FilterTest, BrightnessAdjustConstructorTest) {
    BrightnessAdjust filter(1.5);
    EXPECT_EQ(filter.getName(), "BrightnessAdjust");
    EXPECT_EQ(filter.getBrightnessFactor(), 1.5);
}

TEST_F(FilterTest, BrightnessAdjustInvalidConstructorTest) {
    EXPECT_THROW(BrightnessAdjust(-1.0), std::invalid_argument);
}

TEST_F(FilterTest, BrightnessAdjustApplyTest) {
    BrightnessAdjust filter(2.0);
    auto result = filter.apply(*test_image);
    
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(FilterTest, BrightnessAdjustParameterTest) {
    BrightnessAdjust filter(1.0);
    
    filter.setBrightnessFactor(2.0);
    EXPECT_EQ(filter.getBrightnessFactor(), 2.0);
    
    EXPECT_THROW(filter.setBrightnessFactor(-1.0), std::invalid_argument);
}

TEST_F(FilterTest, BrightnessAdjustCloneTest) {
    BrightnessAdjust original(1.5);
    auto cloned = original.clone();
    
    EXPECT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->getName(), "BrightnessAdjust");
}

TEST_F(FilterTest, FilterPolymorphismTest) {
    std::vector<std::unique_ptr<Filter>> filters;
    
    filters.push_back(std::make_unique<GaussianBlur>(1.0, 3));
    filters.push_back(std::make_unique<EdgeDetection>());
    filters.push_back(std::make_unique<BrightnessAdjust>(1.5));
    
    for (const auto& filter : filters) {
        auto result = filter->apply(*test_image);
        EXPECT_NE(result, nullptr);
        EXPECT_FALSE(filter->getName().empty());
    }
}

TEST_F(FilterTest, FilterParametersTest) {
    GaussianBlur filter(1.0, 5);
    
    // Test parameter setting
    std::map<std::string, double> params = {{"sigma", 2.0}, {"kernel_size", 7.0}};
    filter.setParameters(params);
    
    auto retrieved_params = filter.getParameters();
    EXPECT_EQ(retrieved_params["sigma"], 2.0);
    EXPECT_EQ(retrieved_params["kernel_size"], 7.0);
}

TEST_F(FilterTest, EdgeDetectionParametersTest) {
    EdgeDetection filter(EdgeDetection::Direction::Horizontal);
    
    std::map<std::string, double> params = {{"direction", 2.0}}; // Both
    filter.setParameters(params);
    
    EXPECT_EQ(filter.getDirection(), EdgeDetection::Direction::Both);
}

TEST_F(FilterTest, BrightnessAdjustParametersTest) {
    BrightnessAdjust filter(1.0);
    
    std::map<std::string, double> params = {{"brightness_factor", 2.5}};
    filter.setParameters(params);
    
    EXPECT_EQ(filter.getBrightnessFactor(), 2.5);
}
