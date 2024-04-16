/// @file transformator.cc
#include "transformator.h"

/// @brief Scales object coordinates by scale
void s21::Transformator::ScaleObj(float scale, std::vector<float> &vertices_) {
  if (scale != 0.0f) {
    for (unsigned i = 0; i < vertices_.size(); i += 3) {
      vertices_[i] *= scale;
      vertices_[i + 1] *= scale;
      vertices_[i + 2] *= scale;
    }
  }
}

/// @brief Moves object by all 3 axes
void s21::Transformator::MoveObj(float x, float y, float z,
                                 std::vector<float> &vertices_) {
  for (unsigned i = 0; i < vertices_.size(); i += 3) {
    vertices_[i] += x / 100;
    vertices_[i + 1] += y / 100;
    vertices_[i + 2] += z / 100;
  }
}

/// @brief Rotates object around 3 axis
void s21::Transformator::RotateObj(float angle_x, float angle_y, float angle_z,
                                   std::vector<float> &vertices_) {
  if (angle_x != 0.0f) {
    RotateX(angle_x, vertices_);
  }
  if (angle_y != 0.0f) {
    RotateY(angle_y, vertices_);
  }
  if (angle_z != 0.0f) {
    RotateZ(angle_z, vertices_);
  }
}

/// @brief Rotates object only around x axis
void s21::Transformator::RotateX(float angle, std::vector<float> &vertices_) {
  float cos_a = std::cos((angle * M_PI / 180.0));
  float sin_a = std::sin((angle * M_PI / 180.0));
  for (unsigned i = 0; i < vertices_.size(); i += 3) {
    float y = vertices_[i + 1];
    float z = vertices_[i + 2];
    vertices_[i + 1] = y * cos_a - z * sin_a;
    vertices_[i + 2] = y * sin_a + z * cos_a;
  }
}

/// @brief Rotates object only around y axis
void s21::Transformator::RotateY(float angle, std::vector<float> &vertices_) {
  float cos_a = std::cos((angle * M_PI / 180.0));
  float sin_a = std::sin((angle * M_PI / 180.0));
  for (unsigned i = 0; i < vertices_.size(); i += 3) {
    float x = vertices_[i];
    float z = vertices_[i + 2];
    vertices_[i] = x * cos_a + z * sin_a;
    vertices_[i + 2] = -x * sin_a + z * cos_a;
  }
}

/// @brief Rotates object only around z axis
void s21::Transformator::RotateZ(float angle, std::vector<float> &vertices_) {
  float cos_a = std::cos((angle * M_PI / 180.0));
  float sin_a = std::sin((angle * M_PI / 180.0));
  for (unsigned i = 0; i < vertices_.size(); i += 3) {
    float x = vertices_[i];
    float y = vertices_[i + 1];
    vertices_[i] = x * cos_a - y * sin_a;
    vertices_[i + 1] = x * sin_a + y * cos_a;
  }
}
