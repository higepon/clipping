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
 public:
  Graphics() : drew_something_(false) {
  }

  void setClip(int x, int y, int w, int h) {
    x_min_ = x;
    x_max_ = x + w;
    y_min_ = y;
    y_max_ = y + h;
  }
  void drawLine(int x1, int y1, int x2, int y2) {
    CohenSutherlandLineClip(x1, y1, x2, y2);
  }

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

 private:
  typedef int OutCode;
  enum {
    INSIDE = 0,  // 0000
    LEFT = 1,    // 0001
    RIGHT = 2,   // 0010
    BOTTOM = 4,  // 0100
    TOP = 8      // 1000
  };

  OutCode ComputeOutCode(double x, double y) {
    OutCode code;
    code = INSIDE;
    if (x < x_min_) {
      code |= LEFT;
    } else if (x > x_max_) {
      code |= RIGHT;
    }
    if (y < y_min_) {
      code |= BOTTOM;
    } else if (y > y_max_) {
      code |= TOP;
    }
    return code;
  }

  // http://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland
  void CohenSutherlandLineClip(double x0, double y0, double x1, double y1) {
    OutCode outcode0 = ComputeOutCode(x0, y0);
    OutCode outcode1 = ComputeOutCode(x1, y1);
    bool accept = false;
    while (true) {
      if (!(outcode0 | outcode1)) {
        // Bitwise OR is 0. Trivially accept and get out of loop
        accept = true;
        break;

      } else if (outcode0 & outcode1) {
        // Bitwise AND is not 0. Trivially reject and get out of loop
        break;
      } else {
        // failed both tests, so calculate the line segment to clip
        // from an outside point to an intersection with clip edge
        double x, y;
        // At least one endpoint is outside the clip rectangle; pick it.
        OutCode outcodeOut = outcode0? outcode0 : outcode1;
        // Now find the intersection point;
        // use formulas y = y0 + slope * (x - x0),
        //              x = x0 + (1 / slope) * (y - y0)
        if (outcodeOut & TOP) {
          // point is above the clip rectangle
          x = x0 + (x1 - x0) * (y_max_ - y0) / (y1 - y0);
          y = y_max_;
        } else if (outcodeOut & BOTTOM) {
          // point is below the clip rectangle
          x = x0 + (x1 - x0) * (y_min_ - y0) / (y1 - y0);
          y = y_min_;
        } else if (outcodeOut & RIGHT) {
          // point is to the right of clip rectangle
          y = y0 + (y1 - y0) * (x_max_- x0) / (x1 - x0);
          x = x_max_;
        } else if (outcodeOut & LEFT) {
          // point is to the left of clip rectangle
          y = y0 + (y1 - y0) * (x_min_ - x0) / (x1 - x0);
          x = x_min_;
        }

        // Now we move outside point to intersection point to clip
        // and get ready for next pass.
        if (outcodeOut == outcode0) {
          x0 = x;
          y0 = y;
          outcode0 = ComputeOutCode(x0, y0);
        } else {
          x1 = x;
          y1 = y;
          outcode1 = ComputeOutCode(x1, y1);
        }
      }
    }
    if (accept) {
      //      DrawRectangle(xmin, ymin, xmax, ymax);
      //      LineSegment(x0, y0, x1, y1);
      markAsDrewSomething();
    }
  }

  bool drew_something_;
  double x_min_;
  double x_max_;
  double y_min_;
  double y_max_;
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

TEST_F(ClippingTest, LineOutsideOfClippedRegionShouldNotBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawLine(20, 20, 40, 40);
  EXPECT_FALSE(g_.drewSomething());
}

TEST_F(ClippingTest, LineInsideOfClippedRegionShouldBeRenderred) {
  g_.setClip(0, 0, 10, 10);
  g_.drawLine(2, 2, 4, 4);
  EXPECT_TRUE(g_.drewSomething());
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
