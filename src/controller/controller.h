#ifndef CPP_3D_VIEWER_V2_SRC_CONTROLLER_CONTROLLER_H_
#define CPP_3D_VIEWER_V2_SRC_CONTROLLER_CONTROLLER_H_

#include "../model/parcer.h"
#include "../model/transformator.h"

namespace s21 {
class Controller {
 public:
  Controller();
  void OpenFile(std::string fname);
  void MoveObj(float x_move, float y_move, float z_move);
  void RotateObj(float rot_x, float rot_y, float rot_z);
  void ScaleObj(float scale);
  std::vector<float> &GetVertices();
  std::vector<unsigned int> &GetFaces();

 private:
  std::unique_ptr<s21::Parser> parser_;
  std::vector<float> *vertices_;
  std::vector<unsigned int> *faces_;
};
};      // namespace s21
#endif  // CPP_3D_VIEWER_V2_CONTROLLER_CONTROLLER_CONTROLLER_H_
