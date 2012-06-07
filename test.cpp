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
#include <vector>
#include <string>
#include "./graphics.h"

namespace {

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
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(0, actions.size());
}

TEST_F(ClippingTest, PixelInsideOfClippedRegionShouldBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawPixel(5, 5);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(1, actions.size());
  ASSERT_EQ(DrawAction(5, 5), actions[0]);
}

TEST_F(ClippingTest, LineOutsideOfClippedRegionShouldNotBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawLine(20, 20, 40, 40);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(0, actions.size());
}

TEST_F(ClippingTest, LineInsideOfClippedRegionShouldBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawLine(2, 2, 4, 4);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(1, actions.size());
  ASSERT_EQ(DrawAction(2, 2, 4, 4), actions[0]);
}

TEST_F(ClippingTest, HorizontalLineShouldBeClipped) {
  g_.setClip(0, 0, 10, 10);
  g_.drawLine(0, 5, 20, 5);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(1, actions.size());
  ASSERT_EQ(DrawAction(0, 5, 10, 5), actions[0]);
}

TEST_F(ClippingTest, VerticalLineShouldBeClipped) {
  g_.setClip(0, 0, 10, 10);
  g_.drawLine(5, 0, 5, 20);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(1, actions.size());
  ASSERT_EQ(DrawAction(5, 0, 5, 10), actions[0]);
}

TEST_F(ClippingTest, NormalLineShouldBeClipped) {
  g_.setClip(5, 5, 10, 10);
  g_.drawLine(0, 0, 20, 20);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(1, actions.size());
  ASSERT_EQ(DrawAction(5, 5, 15, 15), actions[0]);
}

TEST_F(ClippingTest, NormalRectangleShouldBeClipped) {
  g_.setClip(0, 0, 10, 10);
  g_.fillRect(0, 0, 2, 2);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(2, actions.size());
  ASSERT_EQ(DrawAction(0, 0, 1, 0), actions[0]);
  ASSERT_EQ(DrawAction(0, 1, 1, 1), actions[1]);
}

TEST_F(ClippingTest, NormalRectangleShouldBeClipped2) {
  g_.setClip(0, 0, 10, 10);
  g_.fillRect(0, 5, 12, 2);
  DrawActions actions = g_.drawActions();
  ASSERT_EQ(2, actions.size());
  ASSERT_EQ(DrawAction(0, 5, 10, 5), actions[0]);
  ASSERT_EQ(DrawAction(0, 6, 10, 6), actions[1]);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
