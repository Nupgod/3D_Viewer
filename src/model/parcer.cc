/// @file parser.cc
#include "parcer.h"

/// @brief Parses .obj file into drawable form
void s21::Parcer::LoadFile(const std::string& f_name) {
  std::ifstream file(f_name);
  /* Проверка файла на существование */
  if (!file.is_open()) {
    throw std::runtime_error("Loading file is not exist or can't be opened");
  }
  faces_.clear();
  vertices_.clear();
  std::stringstream ss;
  std::string line = "", prefix = "";
  float vx = 0, vy = 0, vz = 0, min_x = 0, min_y = 0, min_z = 0, max_x = 0,
        max_y = 0, max_z = 0;
  /* Построчное считывания данных из файла
  с помощью строкого потока данных */
  while (std::getline(file, line)) {
    ss.str(line);
    ss >> prefix;
    if (prefix == "v") {
      ss >> vx >> vy >> vz;
      MinMax(vx, vy, vz, min_x, min_y, min_z, max_x, max_y, max_z);
      vertices_.push_back(vx);
      vertices_.push_back(vy);
      vertices_.push_back(vz);
    } else if (prefix == "f") {
      ss >> line;
      std::size_t pos = line.find('/');
      unsigned int begin = std::stoul(line.substr(0, pos), nullptr, 10) - 1;
      faces_.push_back(begin);
      while (ss >> line) {
        pos = line.find('/');
        faces_.push_back(std::stoul(line.substr(0, pos), nullptr, 10) - 1);
        faces_.push_back(std::stoul(line.substr(0, pos), nullptr, 10) - 1);
      }
      faces_.push_back(begin);
    }
    prefix.clear();
    ss.clear();
  }
  Normalize(min_x, min_y, min_z, max_x, max_y, max_z);
  file.close();
}

/// @brief Service funcition. Finds minimum and maximum coordinates in array of
/// such
void s21::Parcer::MinMax(float x, float y, float z, float& min_x, float& min_y,
                         float& min_z, float& max_x, float& max_y,
                         float& max_z) noexcept {
  if (x < min_x) {
    min_x = x;
  } else if (x > max_x) {
    max_x = x;
  }
  if (y < min_y) {
    min_y = y;
  } else if (y > max_y) {
    max_y = y;
  }
  if (z < min_z) {
    min_z = z;
  } else if (z > max_z) {
    max_z = z;
  }
}

/// @brief Service function. Puts model in the center of coordinates and scales
/// it to perfectly fit 2*2*2 cube
void s21::Parcer::Normalize(float min_x, float min_y, float min_z, float max_x,
                            float max_y, float max_z) noexcept {
  float nx = (min_x + max_x) / 2, ny = (min_y + max_y) / 2,
        nz = (min_z + max_z) / 2;
  float abs_max = std::max({std::abs(min_x), std::abs(min_y), std::abs(min_z),
                            std::abs(max_x), std::abs(max_y), std::abs(max_z)});
  for (unsigned i = 0; i < vertices_.size(); i += 3) {
    vertices_[i] = (vertices_[i] - nx) / abs_max;
    vertices_[i + 1] = (vertices_[i + 1] - ny) / abs_max;
    vertices_[i + 2] = (vertices_[i + 2] - nz) / abs_max;
  }
}
/// @brief Passes vertices position to controller
std::vector<float>& s21::Parcer::GetVertices() noexcept { return vertices_; }
/// @brief Passes indices position to controller
std::vector<unsigned int>& s21::Parcer::GetFaces() noexcept { return faces_; }
