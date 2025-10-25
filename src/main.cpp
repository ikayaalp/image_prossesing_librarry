#include "Image.h"
#include "GaussianBlur.h"
#include "EdgeDetection.h"
#include "BrightnessAdjust.h"
#include "FilterPipeline.h"
#include "ImageViewer.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace ImageProcessing;

int main() {
    std::cout << "=== Modern C++ Image Processing Library Demo ===\n\n";
    
    // Create ImageViewer instance
    ImageViewer viewer;
    
    try {
        // Create a test image
        std::cout << "1. Creating test image...\n";
        Image test_image(200, 200, 1);
        
        // Create a pattern
        for (int y = 0; y < 200; ++y) {
            for (int x = 0; x < 200; ++x) {
                // Create a circular pattern
                int center_x = 100, center_y = 100;
                double distance = std::sqrt((x - center_x) * (x - center_x) + 
                                           (y - center_y) * (y - center_y));
                
                if (distance < 50) {
                    test_image.at(x, y) = 255; // White circle
                } else if (distance < 70) {
                    test_image.at(x, y) = 128; // Gray ring
                } else {
                    test_image.at(x, y) = 0; // Black background
                }
            }
        }
        
        std::cout << "   ✓ Test image created (" 
                  << test_image.getWidth() << "x" << test_image.getHeight() << ")\n";
        
        // Display original image
        viewer.imshow("Original Image", test_image);
        std::cout << "   ✓ Original image displayed in window\n";
        
        // Demonstrate polymorphism with different filters
        std::cout << "\n2. Applying individual filters...\n";
        
        // Gaussian Blur
        std::cout << "   - Applying Gaussian Blur...\n";
        auto gaussian_filter = std::make_unique<GaussianBlur>(2.0, 5);
        auto blurred_image = gaussian_filter->apply(test_image);
        viewer.imshow("Gaussian Blur", *blurred_image);
        std::cout << "     ✓ Gaussian blur applied and displayed\n";
        
        // Edge Detection
        std::cout << "   - Applying Edge Detection...\n";
        auto edge_filter = std::make_unique<EdgeDetection>(EdgeDetection::Direction::Both);
        auto edge_image = edge_filter->apply(test_image);
        viewer.imshow("Edge Detection", *edge_image);
        std::cout << "     ✓ Edge detection applied and displayed\n";
        
        // Brightness Adjustment
        std::cout << "   - Applying Brightness Adjustment...\n";
        auto brightness_filter = std::make_unique<BrightnessAdjust>(1.5);
        auto bright_image = brightness_filter->apply(test_image);
        viewer.imshow("Brightness Adjustment", *bright_image);
        std::cout << "     ✓ Brightness adjustment applied and displayed\n";
        
        // Demonstrate filter pipeline
        std::cout << "\n3. Creating filter pipeline...\n";
        FilterPipeline pipeline;
        
        // Add filters to pipeline
        pipeline.addFilter(std::make_unique<GaussianBlur>(1.0, 3));
        pipeline.addFilter(std::make_unique<BrightnessAdjust>(1.2));
        pipeline.addFilter(std::make_unique<EdgeDetection>());
        
        std::cout << "   ✓ Pipeline created with " << pipeline.getFilterCount() << " filters\n";
        
        // Apply pipeline
        std::cout << "   - Applying filter pipeline...\n";
        auto pipeline_result = pipeline.apply(test_image);
        viewer.imshow("Filter Pipeline Result", *pipeline_result);
        std::cout << "     ✓ Pipeline applied and displayed\n";
        
        // Demonstrate STL algorithms and modern C++ features
        std::cout << "\n4. Image analysis using STL algorithms...\n";
        
        // Calculate statistics
        auto min_val = test_image.getMinValue();
        auto max_val = test_image.getMaxValue();
        double mean_val = test_image.getMeanValue();
        double std_dev = test_image.getStandardDeviation();
        
        std::cout << "   ✓ Image statistics:\n";
        std::cout << "     - Min value: " << (min_val ? std::to_string(*min_val) : "N/A") << "\n";
        std::cout << "     - Max value: " << (max_val ? std::to_string(*max_val) : "N/A") << "\n";
        std::cout << "     - Mean value: " << mean_val << "\n";
        std::cout << "     - Standard deviation: " << std_dev << "\n";
        
        // Demonstrate histogram
        std::vector<int> histogram;
        test_image.histogram(histogram);
        
        // Find most frequent pixel value
        auto max_hist_it = std::max_element(histogram.begin(), histogram.end());
        int most_frequent_value = std::distance(histogram.begin(), max_hist_it);
        std::cout << "     - Most frequent pixel value: " << most_frequent_value 
                  << " (count: " << *max_hist_it << ")\n";
        
        // Demonstrate modern C++17 features
        std::cout << "\n5. Modern C++17 features demonstration...\n";
        
        // Optional usage
        auto pixel_value = test_image.getPixel(100, 100);
        if (pixel_value) {
            std::cout << "   ✓ Pixel at (100,100): " << *pixel_value << "\n";
        }
        
        // Lambda functions
        auto apply_function = [](Image::Pixel p) -> Image::Pixel {
            return static_cast<Image::Pixel>(std::min(255, static_cast<int>(p * 1.2)));
        };
        
        Image modified_image = test_image;
        modified_image.applyFunction(apply_function);
        viewer.imshow("Lambda Modified Image", modified_image);
        std::cout << "   ✓ Lambda function applied and displayed\n";
        
        // Demonstrate filter cloning
        std::cout << "\n6. Demonstrating filter cloning...\n";
        auto cloned_filter = gaussian_filter->clone();
        auto cloned_result = cloned_filter->apply(test_image);
        viewer.imshow("Cloned Filter Result", *cloned_result);
        std::cout << "   ✓ Filter cloned and displayed\n";
        
        // Pipeline information
        std::cout << "\n7. Pipeline information:\n";
        auto filter_names = pipeline.getAllFilterNames();
        for (size_t i = 0; i < filter_names.size(); ++i) {
            std::cout << "   - Filter " << i << ": " << filter_names[i] << "\n";
        }
        
        std::cout << "\n=== Demo completed successfully! ===\n";
        std::cout << "All images are displayed in separate windows.\n";
        std::cout << "Press any key in any window to close all windows and exit.\n";
        
        // Wait for user input before closing
        viewer.waitKey(0);
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
