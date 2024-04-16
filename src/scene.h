#ifndef CPP4_3DVIEWER_SRC_SCENE_H_
#define CPP4_3DVIEWER_SRC_SCENE_H_

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QSettings>
#include <vector>

#include "mainwindow.h"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  Scene(QWidget *);
  ~Scene();

  std::vector<float> *vertices_;
  std::vector<unsigned> *indices_;
  int vertices_count;
  int indices_count;
  QColor lineCol = {0, 0, 255};  // Blue
  QColor vecCol = {255, 0, 0};   // Red
  QColor bgCol = {0, 0, 0};      // Black
  int point = 0;
  int line = 0;
  int perspective = 0;
  float lineWidth = 0.0045f;
  float pointSize = 0.0045f;

 protected:
  void initializeGL() override;
  void paintGL() override;

 private:
  void compainLineShader();
  void compainPointShader();

  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer vbo_;
  QOpenGLBuffer ebo_;
  QOpenGLShaderProgram lineProgram_;
  QOpenGLShaderProgram pointProgram_;
  QMatrix4x4 perspectiveM_;
};

#endif  // CPP4_3DVIEWER_SRC_SCENE_H_
