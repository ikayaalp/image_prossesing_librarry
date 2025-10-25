#include <gtest/gtest.h>
#include "FilterPipeline.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "Image.h"

using namespace GorselIsleme;

class PipelineTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_image = std::make_unique<Image>(40, 40, 1);
        
        // Test pattern
        for (int y = 0; y < 40; ++y) {
            for (int x = 0; x < 40; ++x) {
                if (x > 15 && x < 25 && y > 15 && y < 25) {
                    test_image->at(x, y) = 255;
                } else {
                    test_image->at(x, y) = 0;
                }
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(PipelineTest, EmptyPipelineTest) {
    FilterPipeline pipeline;
    auto result = pipeline.apply(*test_image);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(PipelineTest, SingleFilterTest) {
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    
    auto result = pipeline.apply(*test_image);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(pipeline.getFilterCount(), 1);
    EXPECT_EQ(pipeline.getFilterName(0), "GaussianBlur");
}

TEST_F(PipelineTest, MultipleFiltersTest) {
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto result = pipeline.apply(*test_image);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(pipeline.getFilterCount(), 3);
    
    auto filter_names = pipeline.getAllFilterNames();
    EXPECT_EQ(filter_names.size(), 3);
    EXPECT_EQ(filter_names[0], "GaussianBlur");
    EXPECT_EQ(filter_names[1], "BrightnessAdjust");
    EXPECT_EQ(filter_names[2], "EdgeDetection");
}

TEST_F(PipelineTest, RemoveFilterTest) {
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    
    EXPECT_EQ(pipeline.getFilterCount(), 2);
    
    pipeline.removeFilter(0);
    EXPECT_EQ(pipeline.getFilterCount(), 1);
    EXPECT_EQ(pipeline.getFilterName(0), "BrightnessAdjust");
}

TEST_F(PipelineTest, ClearPipelineTest) {
    FilterPipeline pipeline;
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    EXPECT_EQ(pipeline.getFilterCount(), 2);
    
    pipeline.clear();
    EXPECT_EQ(pipeline.getFilterCount(), 0);
}

TEST_F(PipelineTest, ClonePipelineTest) {
    FilterPipeline original_pipeline;
    original_pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    original_pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    auto cloned_pipeline = original_pipeline.clone();
    ASSERT_NE(cloned_pipeline, nullptr);
    
    EXPECT_EQ(cloned_pipeline->getFilterCount(), 2);
    EXPECT_EQ(cloned_pipeline->getFilterName(0), "GaussianBlur");
    EXPECT_EQ(cloned_pipeline->getFilterName(1), "BrightnessAdjust");
}