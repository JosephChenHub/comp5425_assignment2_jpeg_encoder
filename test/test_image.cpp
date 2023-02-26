#include <gtest/gtest.h>
#include <vector>
#include <random>

#include "image.hpp"
using namespace std;

// The fixture for testing class Image.
class ImageTest : public ::testing::Test {
 protected:
  // Set up the test fixture.
  virtual void SetUp() {
    // code to run before each test case
  }

  // Tear down the test fixture.
  virtual void TearDown() {
    // code to run after each test case
  }

  // Objects declared here can be used by all tests in the test case for Image.

  template <typename Dtype>
  static Image<Dtype> get_random_image(const int min, const int max) {
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<int> dis1(min, max);
     std::uniform_int_distribution<int> dis2(1, 32);
     int h = dis1(gen);
     int w = dis1(gen);
     int c = dis2(gen);
     
     std::cout << "===== generate random image with shape:"
               << "(" <<h  << ", " << w <<", "<< c << ")"
               << std::endl;
     Image<Dtype> img(h, w, c);
     std::uniform_int_distribution<Dtype> dis3(0, 255);
     for(size_t i = 0; i < h*w*c; ++i) img.data()[i] = dis3(gen);
    
     return img;
  }
};

// Test case for Image constructors
TEST_F(ImageTest, constructors) {
  Image<uint8_t> img1;
  Image<uint8_t> img2 = ImageTest::get_random_image<uint8_t>(1, 4000); // [1,4000] 
  
  img1 = img2;
  ASSERT_EQ(img1.rows(), img2.rows());
  ASSERT_EQ(img1.cols(), img2.cols());
  ASSERT_EQ(img1.channels(), img2.channels());
  ASSERT_EQ(img1.numel(), img2.numel());
  
  for(size_t i = 0; i < img1.numel(); ++i) ASSERT_EQ(img1.data()[i], img2.data()[i]);

  Image<uint8_t> img3(10, 5, 7);
  ASSERT_EQ(img3.rows(), 10);
  ASSERT_EQ(img3.cols(), 5);
  ASSERT_EQ(img3.channels(), 7);
 
  img3 = img1;
  ASSERT_EQ(img3.rows(), img1.rows());
  ASSERT_EQ(img3.cols(), img1.cols());
  ASSERT_EQ(img3.channels(), img1.channels());

  ASSERT_EQ(img3(0, 0, 0), img1(0, 0, 0));
}

TEST_F(ImageTest, read_from_file) {
  // from image file
  Image<uint8_t> img("./data/sg_0.png");
  ASSERT_EQ(img.rows(), 4000);
  ASSERT_EQ(img.cols(), 6000);
  ASSERT_EQ(img.channels(), 3);
}

TEST_F(ImageTest, copyFrom) {
  Image<uint8_t> img1 = ImageTest::get_random_image<uint8_t>(1, 4000);
  Image<float> img2;
  img2.copyFrom(img1); // uint8_t -> float
  const float EPS = 1e-7;
  for(size_t i = 0; i < img2.numel(); ++i) ASSERT_LE(img2.data()[i] - img1.data()[i], EPS);

  // float -> uint8_t
  Image<uint8_t> img3;
  img3.copyFrom(img2);
 
  for(size_t i = 0; i < img2.numel(); ++i) ASSERT_LE(img2.data()[i] - img1.data()[i], 1.0);
}

TEST_F(ImageTest, roi_sampling) {

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

