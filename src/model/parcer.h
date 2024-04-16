#ifndef CPP_3D_VIEWER_V2_SRC_MODEL_PARCER_H_
#define CPP_3D_VIEWER_V2_SRC_MODEL_PARCER_H_

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

// Abstract Parser class
class Parser {
 public:
  virtual void LoadFile(const std::string& f_name) = 0;
  virtual void MinMax(float x, float y, float z, float& minX, float& minY,
                      float& minZ, float& maxX, float& maxY,
                      float& maxZ) noexcept = 0;
  virtual void Normalize(float minX, float minY, float minZ, float maxX,
                         float maxY, float maxZ) noexcept = 0;
  virtual std::vector<float>& GetVertices() noexcept = 0;
  virtual std::vector<unsigned int>& GetFaces() noexcept = 0;
  virtual ~Parser() {}
};

// Concrete Parser class
class Parcer : public Parser {
 public:
  Parcer() : vertices_(0), faces_(0) {}
  void LoadFile(const std::string& f_name) override;
  void MinMax(float x, float y, float z, float& minX, float& minY, float& minZ,
              float& maxX, float& maxY, float& maxZ) noexcept override;
  void Normalize(float minX, float minY, float minZ, float maxX, float maxY,
                 float maxZ) noexcept override;
  std::vector<float>& GetVertices() noexcept override;
  std::vector<unsigned int>& GetFaces() noexcept override;
  ~Parcer() override {}

 private:
  std::vector<float> vertices_;
  std::vector<unsigned int> faces_;
};

// Factory class
class ParserFactory {
 public:
  static std::unique_ptr<Parser> CreateParser() {
    return std::make_unique<Parcer>();
  }
};

};      // namespace s21
#endif  // CPP_3D_VIEWER_V2_SRC_MODEL_PARCER_H_
