#include <gtest/gtest.h>
#include "FilterPipeline.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"

using namespace ImageProcessing;

class PipelineTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_image = std::make_unique<Image>(10, 10, 1);
        
        // Create simple test pattern
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                test_image->at(x, y) = static_cast<Image::Pixel>(x * 10 + y);
            }
        }
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(PipelineTest, EmptyPipelineTest) {
    FilterPipeline pipeline;
    EXPECT_EQ(pipeline.getFilterCount(), 0);
    
    auto result = pipeline.apply(*test_image);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(PipelineTest, AddFilterTest) {
    FilterPipeline pipeline;
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    EXPECT_EQ(pipeline.getFilterCount(), 1);
    
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    EXPECT_EQ(pipeline.getFilterCount(), 2);
}

TEST_F(PipelineTest, AddNullFilterTest) {
    FilterPipeline pipeline;
    
    EXPECT_THROW(pipeline.addFilter(nullptr), std::invalid_argument);
}

TEST_F(PipelineTest, RemoveFilterTest) {
    FilterPipeline pipeline;
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    EXPECT_EQ(pipeline.getFilterCount(), 2);
    
    pipeline.removeFilter(0);
    EXPECT_EQ(pipeline.getFilterCount(), 1);
    
    pipeline.removeFilter(0);
    EXPECT_EQ(pipeline.getFilterCount(), 0);
}

TEST_F(PipelineTest, RemoveInvalidFilterTest) {
    FilterPipeline pipeline;
    
    EXPECT_THROW(pipeline.removeFilter(0), std::out_of_range);
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    EXPECT_THROW(pipeline.removeFilter(1), std::out_of_range);
}

TEST_F(PipelineTest, ApplyPipelineTest) {
    FilterPipeline pipeline;
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    auto result = pipeline.apply(*test_image);
    
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}

TEST_F(PipelineTest, GetFilterNameTest) {
    FilterPipeline pipeline;
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    EXPECT_EQ(pipeline.getFilterName(0), "GaussianBlur");
    EXPECT_EQ(pipeline.getFilterName(1), "EdgeDetection");
}

TEST_F(PipelineTest, GetInvalidFilterNameTest) {
    FilterPipeline pipeline;
    
    EXPECT_THROW(pipeline.getFilterName(0), std::out_of_range);
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    EXPECT_THROW(pipeline.getFilterName(1), std::out_of_range);
}

TEST_F(PipelineTest, GetAllFilterNamesTest) {
    FilterPipeline pipeline;
    
    auto names = pipeline.getAllFilterNames();
    EXPECT_TRUE(names.empty());
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    names = pipeline.getAllFilterNames();
    EXPECT_EQ(names.size(), 3);
    EXPECT_EQ(names[0], "GaussianBlur");
    EXPECT_EQ(names[1], "EdgeDetection");
    EXPECT_EQ(names[2], "BrightnessAdjust");
}

TEST_F(PipelineTest, SwapFiltersTest) {
    FilterPipeline pipeline;
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    
    auto names_before = pipeline.getAllFilterNames();
    EXPECT_EQ(names_before[0], "GaussianBlur");
    EXPECT_EQ(names_before[1], "EdgeDetection");
    
    pipeline.swapFilters(0, 1);
    
    auto names_after = pipeline.getAllFilterNames();
    EXPECT_EQ(names_after[0], "EdgeDetection");
    EXPECT_EQ(names_after[1], "GaussianBlur");
}

TEST_F(PipelineTest, SwapInvalidFiltersTest) {
    FilterPipeline pipeline;
    
    EXPECT_THROW(pipeline.swapFilters(0, 1), std::out_of_range);
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    EXPECT_THROW(pipeline.swapFilters(0, 1), std::out_of_range);
    EXPECT_THROW(pipeline.swapFilters(1, 0), std::out_of_range);
}

TEST_F(PipelineTest, InsertFilterTest) {
    FilterPipeline pipeline;
    
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    pipeline.insertFilter(1, std::make_unique<EdgeDetection>());
    
    auto names = pipeline.getAllFilterNames();
    EXPECT_EQ(names.size(), 3);
    EXPECT_EQ(names[0], "GaussianBlur");
    EXPECT_EQ(names[1], "EdgeDetection");
    EXPECT_EQ(names[2], "BrightnessAdjust");
}

TEST_F(PipelineTest, InsertInvalidFilterTest) {
    FilterPipeline pipeline;
    
    EXPECT_THROW(pipeline.insertFilter(1, std::make_unique<GaussianBlur>(1.0, 3)), 
                 std::out_of_range);
    EXPECT_THROW(pipeline.insertFilter(0, nullptr), std::invalid_argument);
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
    FilterPipeline original;
    
    original.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    original.addFilter(std::make_unique<EdgeDetection>());
    original.addFilter(std::make_unique<BrightnessAdjust>(1.5));
    
    auto cloned = original.clone();
    
    EXPECT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->getFilterCount(), 3);
    
    auto original_names = original.getAllFilterNames();
    auto cloned_names = cloned->getAllFilterNames();
    
    EXPECT_EQ(original_names.size(), cloned_names.size());
    for (size_t i = 0; i < original_names.size(); ++i) {
        EXPECT_EQ(original_names[i], cloned_names[i]);
    }
}

TEST_F(PipelineTest, ComplexPipelineTest) {
    FilterPipeline pipeline;
    
    // Add multiple filters
    pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
    pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
    pipeline.addFilter(std::make_unique<EdgeDetection>());
    pipeline.addFilter(std::make_unique<GaussianBlur>(0.5, 3));
    
    EXPECT_EQ(pipeline.getFilterCount(), 4);
    
    auto result = pipeline.apply(*test_image);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->getWidth(), test_image->getWidth());
    EXPECT_EQ(result->getHeight(), test_image->getHeight());
}
