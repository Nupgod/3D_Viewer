#include <gtest/gtest.h>

#include "controller/controller.h"

TEST(Parcer, Default1) {
  s21::Controller newControl;
  std::string path = "../datasets/piramid.obj";
  newControl.OpenFile(path);
  EXPECT_EQ((int)newControl.GetVertices().size() / 3, 21);
  EXPECT_EQ((int)newControl.GetFaces().size() / 3, 53);
}

TEST(Transformator, Default1) {
  s21::Controller Control;
  std::string path = "../datasets/piramid.obj";
  float normalCoords[] = {-0.974829f, -0.865561f, -0.974829f};
  Control.OpenFile(path);
  const float* p = Control.GetVertices().data();
  for (std::size_t i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(p[i], normalCoords[i]);
  }
  Control.MoveObj(5.0f, 2.5f, 3.5f);
  float moveCoords[] = {normalCoords[0] + 5.0f / 100.0f,
                        normalCoords[1] + 2.5f / 100.0f,
                        normalCoords[2] + 3.5f / 100.0f};
  for (std::size_t i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(p[i], moveCoords[i]);
  }
  Control.ScaleObj(0.5f);
  float ScaleCoords[] = {moveCoords[0] * 0.5f, moveCoords[1] * 0.5f,
                         moveCoords[2] * 0.5f};
  for (std::size_t i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(p[i], ScaleCoords[i]);
  }
  Control.RotateObj(30.0f, 30.0f, 30.0f);
  float cosA = std::cos((30.0f * M_PI / 180.0));
  float sinA = std::sin((30.0f * M_PI / 180.0));
  float x = ScaleCoords[0], y = ScaleCoords[1], z = ScaleCoords[2];
  ScaleCoords[1] = y * cosA - z * sinA;
  ScaleCoords[2] = y * sinA + z * cosA;

  x = ScaleCoords[0], y = ScaleCoords[1], z = ScaleCoords[2];
  ScaleCoords[0] = x * cosA + z * sinA;
  ScaleCoords[2] = -x * sinA + z * cosA;

  x = ScaleCoords[0], y = ScaleCoords[1], z = ScaleCoords[2];
  ScaleCoords[0] = x * cosA - y * sinA;
  ScaleCoords[1] = x * sinA + y * cosA;

  for (std::size_t i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(p[i], ScaleCoords[i]);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
