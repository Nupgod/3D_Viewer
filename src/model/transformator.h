#ifndef CPP_3D_VIEWER_V2_MODEL_TRANSFORMATOR_H_
#define CPP_3D_VIEWER_V2_MODEL_TRANSFORMATOR_H_

#include <cmath>
#include <vector>

namespace s21 {
class Transformator {
 public:
  // Singleton instance access method
  static Transformator &GetInstance() {
    static Transformator transformator;
    return transformator;
  }
  void MoveObj(float x, float y, float z, std::vector<float> &vertices_);
  void RotateObj(float x, float y, float z, std::vector<float> &vertices_);
  void ScaleObj(float zoom, std::vector<float> &vertices_);
  void RotateX(float angle, std::vector<float> &vertices_);
  void RotateY(float angle, std::vector<float> &vertices_);
  void RotateZ(float angle, std::vector<float> &vertices_);

 private:
  Transformator(){};
  ~Transformator(){};
};
};      // namespace s21
#endif  // CPP_3D_VIEWER_V2_MODEL_TRANSFORMATOR_H_
