/*
 *   Copyright (c) 2012  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdint.h>
#include <gtest/gtest.h>

namespace {

class Graphics {
 private:
  int x_min_;
  int x_max_;
  int y_min_;
  int y_max_;
 public:
  void setClip(int x, int y, int w, int h) {
    x_min_ = x;
    x_max_ = x + w;
    y_min_ = y;
    y_max_ = y + h;
  }
  void drawLine(int x1, int y1, int x2, int y2) {}

  void drawPixel(int x, int y) {
    if (x >= x_min_ && x <= x_max_ && y >= y_min_ && y <= y_max_) {
      markAsDrewSomething();
    }
  }

  // for testability
  bool drewSomething() const {
    return drew_something_;
  }
 private:
  void markAsDrewSomething() {
    drew_something_ = true;
  }

  bool drew_something_;
};

class ClippingTest : public ::testing::Test {
 protected:
  Graphics g_;
 public:
  ClippingTest() {
  }

  virtual ~ClippingTest() {
  }
};

TEST_F(ClippingTest, PixelOutsideOfClippedRegionShouldNotBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawPixel(20, 20);
  EXPECT_FALSE(g_.drewSomething());
}

TEST_F(ClippingTest, PixelInsideOfClippedRegionShouldBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawPixel(5, 5);
  EXPECT_TRUE(g_.drewSomething());
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
