/// @file controller.cc
#include "controller.h"

/// @brief Default constructor
s21::Controller::Controller() {
  parser_ = s21::ParserFactory::CreateParser();
  vertices_ = &(parser_->GetVertices());
  faces_ = &(parser_->GetFaces());
};

/// @brief passes the filename for parser to open
void s21::Controller::OpenFile(std::string fname) {
  //  try {
  parser_->LoadFile(fname);
  //  } catch (const std::runtime_error& err) {
  //    throw err;
  //  }
}

/// @brief Passes movement info to model
void s21::Controller::MoveObj(float move_x, float move_y, float move_z) {
  Transformator& transformator = Transformator::GetInstance();
  transformator.MoveObj(move_x, move_y, move_z, *vertices_);
}

/// @brief Passes rotation info to model
void s21::Controller::RotateObj(float rot_x, float rot_y, float rot_z) {
  Transformator& transformator = Transformator::GetInstance();
  transformator.RotateObj(rot_x, rot_y, rot_z, *vertices_);
}

/// @brief Passes scaling info to model
void s21::Controller::ScaleObj(float scale) {
  Transformator& transformator = Transformator::GetInstance();
  transformator.ScaleObj(scale, *vertices_);
}

/// @brief Connects model to view (vertices info)
std::vector<float>& s21::Controller::GetVertices() { return *vertices_; }
/// @brief Connects model to view (indices info)
std::vector<unsigned int>& s21::Controller::GetFaces() { return *faces_; }
