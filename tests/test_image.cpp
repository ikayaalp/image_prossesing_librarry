#include <gtest/gtest.h>
#include "Image.h"

using namespace GorselIsleme;

class ImageTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_image = std::make_unique<Image>(100, 100, 1);
    }
    
    std::unique_ptr<Image> test_image;
};

TEST_F(ImageTest, ConstructorTest) {
    EXPECT_EQ(test_image->getWidth(), 100);
    EXPECT_EQ(test_image->getHeight(), 100);
    EXPECT_EQ(test_image->getChannels(), 1);
    EXPECT_FALSE(test_image->empty());
}

TEST_F(ImageTest, PixelAccessTest) {
    test_image->at(50, 50) = 255;
    EXPECT_EQ(test_image->at(50, 50), 255);
}

TEST_F(ImageTest, SaveLoadTest) {
    test_image->at(10, 10) = 128;
    EXPECT_TRUE(test_image->save("test_image.pgm"));
    
    auto loaded_image = Image::load("test_image.pgm");
    ASSERT_NE(loaded_image, nullptr);
    EXPECT_EQ(loaded_image->getWidth(), 100);
    EXPECT_EQ(loaded_image->getHeight(), 100);
}

TEST_F(ImageTest, CopyConstructorTest) {
    test_image->at(25, 25) = 200;
    Image copy_image(*test_image);
    
    EXPECT_EQ(copy_image.getWidth(), test_image->getWidth());
    EXPECT_EQ(copy_image.getHeight(), test_image->getHeight());
    EXPECT_EQ(copy_image.at(25, 25), 200);
}

TEST_F(ImageTest, AssignmentOperatorTest) {
    test_image->at(30, 30) = 150;
    Image assigned_image(50, 50, 1);
    assigned_image = *test_image;
    
    EXPECT_EQ(assigned_image.getWidth(), test_image->getWidth());
    EXPECT_EQ(assigned_image.getHeight(), test_image->getHeight());
    EXPECT_EQ(assigned_image.at(30, 30), 150);
}