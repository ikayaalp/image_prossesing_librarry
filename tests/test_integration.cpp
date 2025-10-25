#include <gtest/gtest.h>
#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include "ImageViewer.h"
#include <memory>
#include <vector>
#include <chrono>

using namespace ImageProcessing;

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test image
        test_image = std::make_unique<Image>(100, 100, 1);
        
        // Create a gradient pattern
        for (int y = 0; y < 100; ++y) {
            for (int x = 0; x < 100; ++x) {
                test_image->at(x, y) = static_cast<Image::Pixel>((x + y) % 256);
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(IntegrationTest, FilterPipelineIntegration) {
    // Create filter pipeline
    FilterPipeline pipeline;
    
    // Add multiple filters
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    // Apply pipeline
    auto result = pipeline.apply(*test_image);
    
    // Verify result
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
    EXPECT_EQ(result->getChannels(), test_image->getChannels());
    
    // Verify pipeline information
    EXPECT_EQ(pipeline.getFilterCount(), 3);
    EXPECT_EQ(pipeline.getFilterName(0), "GaussianBlur");
    EXPECT_EQ(pipeline.getFilterName(1), "BrightnessAdjust");
    EXPECT_EQ(pipeline.getFilterName(2), "EdgeDetection");
}

TEST_F(IntegrationTest, FilterCloningIntegration) {
    // Create original filter
    auto original_filter = std::make_unique<GaussianBlur>(2.0, 5);
    
    // Clone filter
    auto cloned_filter = original_filter->clone();
    
    // Apply both filters
    auto original_result = original_filter->apply(*test_image);
    auto cloned_result = cloned_filter->apply(*test_image);
    
    // Results should be identical
    ASSERT_NE(original_result, nullptr);
    ASSERT_NE(cloned_result, nullptr);
    
    // Compare pixel values (should be identical)
    for (int y = 0; y < test_image->getHeight(); ++y) {
        for (int x = 0; x < test_image->getWidth(); ++x) {
            EXPECT_EQ(original_result->at(x, y), cloned_result->at(x, y));
        }
    }
}

TEST_F(IntegrationTest, FilterParameterIntegration) {
    // Create filter with parameters
    auto filter = std::make_unique<GaussianBlur>(1.5, 7);
    
    // Get initial parameters
    auto params = filter->getParameters();
    EXPECT_EQ(params["sigma"], 1.5);
    EXPECT_EQ(params["kernel_size"], 7);
    
    // Change parameters
    std::map<std::string, double> new_params = {
        {"sigma", 2.0},
        {"kernel_size", 9}
    };
    filter->setParameters(new_params);
    
    // Verify parameters changed
    auto updated_params = filter->getParameters();
    EXPECT_EQ(updated_params["sigma"], 2.0);
    EXPECT_EQ(updated_params["kernel_size"], 9);
}

TEST_F(IntegrationTest, ImageViewerIntegration) {
    // Create ImageViewer (should not crash)
    ImageViewer viewer;
    
    // Test window creation and display
    viewer.imshow("Test Window", *test_image);
    
    // Verify window was created
    EXPECT_TRUE(viewer.isWindowOpen("Test Window"));
    
    // Test window title update
    viewer.setWindowTitle("Test Window", "Updated Title");
    
    // Test window resize
    viewer.resizeWindow("Test Window", 200, 200);
    
    // Test image update
    viewer.updateImage("Test Window", *test_image);
    
    // Clean up
    viewer.destroyWindow("Test Window");
    EXPECT_FALSE(viewer.isWindowOpen("Test Window"));
}

TEST_F(IntegrationTest, ComplexFilterChainIntegration) {
    // Create complex filter chain
    FilterPipeline pipeline;
    
    // Add multiple filters with different parameters
    pipeline.addFilter(std::make_unique<GaussianBlur>(0.5, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(0.8));
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 5));
    pipeline.addFilter(std::make_unique<EdgeDetection>(EdgeDetection::Direction::Horizontal));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    // Apply pipeline
    auto result = pipeline.apply(*test_image);
    
    // Verify result
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
    
    // Verify all filters were applied
    EXPECT_EQ(pipeline.getFilterCount(), 5);
    
    // Get all filter names
    auto filter_names = pipeline.getAllFilterNames();
    EXPECT_EQ(filter_names.size(), 5);
    EXPECT_EQ(filter_names[0], "GaussianBlur");
    EXPECT_EQ(filter_names[1], "BrightnessAdjust");
    EXPECT_EQ(filter_names[2], "GaussianBlur");
    EXPECT_EQ(filter_names[3], "EdgeDetection");
    EXPECT_EQ(filter_names[4], "BrightnessAdjust");
}


TEST_F(IntegrationTest, PerformanceIntegration) {
    // Create larger image for performance test
    Image large_image(500, 500, 1);
    
    // Fill with pattern
    for (int y = 0; y < 500; ++y) {
        for (int x = 0; x < 500; ++x) {
            large_image.at(x, y) = static_cast<Image::Pixel>((x * y) % 256);
        }
    }
    
    // Create pipeline with multiple filters
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 5));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    
    // Measure time (basic performance test)
    auto start = std::chrono::high_resolution_clock::now();
    auto result = pipeline.apply(large_image);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 5 seconds)
    EXPECT_LT(duration.count(), 5000);
    
    // Verify result
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), 500);
    EXPECT_EQ(result->getHeight(), 500);
}
