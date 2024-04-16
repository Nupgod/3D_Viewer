/// @file scene.cc
#include "scene.h"

Scene::Scene(QWidget *parent) : QOpenGLWidget(parent) {
  vertices_count = 0;
  indices_count = 0;
  vertices_ = nullptr;
  indices_ = nullptr;
  updateBehavior();
}

/// @brief (override) Function is called on widget creation
void Scene::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_NORMALIZE);
  if (!vao_.isCreated()) {
    vao_.create();  // это называется Vertex Array Object
    vao_.bind();
    pointProgram_.create();
    lineProgram_.create();
  }
}

/// @brief (override) Function is called on widget update/redraw action
void Scene::paintGL() {
  // OpenGL умеет читать только из своих буферов, поэтому:
  glClearColor(bgCol.redF(), bgCol.greenF(), bgCol.blueF(), bgCol.alphaF());
  if (vertices_count != 0 && indices_count != 0) {
    compainLineShader();
    compainPointShader();

    vbo_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo_.create();
    vbo_.setUsagePattern(QOpenGLBuffer::StreamDraw);
    vbo_.bind();
    vbo_.allocate(vertices_->data(), vertices_count * sizeof(float));

    lineProgram_.bind();
    lineProgram_.enableAttributeArray("position");
    lineProgram_.setAttributeArray("position", GL_FLOAT, 0, 3);

    ebo_ = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo_.create();
    ebo_.bind();
    ebo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ebo_.allocate(indices_->data(), indices_count * sizeof(unsigned));

    if (perspective == 1) {
      perspectiveM_.setToIdentity();
      perspectiveM_.frustum(-1, 1, -1, 1, 0.25, 3.5);
      perspectiveM_.translate(0, 0, -1.0);
      lineProgram_.setUniformValue(lineProgram_.uniformLocation("persp"),
                                   perspectiveM_);
      lineProgram_.setUniformValue(lineProgram_.uniformLocation("perspV"),
                                   perspective);
    }

    lineProgram_.setUniformValue(lineProgram_.uniformLocation("lWidth"),
                                 lineWidth);
    lineProgram_.setUniformValue(lineProgram_.uniformLocation("lCol"), lineCol);
    glDrawElements(GL_LINES, indices_count, GL_UNSIGNED_INT, nullptr);
    lineProgram_.release();
    lineProgram_.removeAllShaders();
    if (point != 0) {
      pointProgram_.bind();
      pointProgram_.setUniformValue(pointProgram_.uniformLocation("persp"),
                                    perspectiveM_);
      pointProgram_.setUniformValue(pointProgram_.uniformLocation("perspV"),
                                    perspective);
      pointProgram_.enableAttributeArray("position");
      pointProgram_.setAttributeBuffer("position", GL_FLOAT, 0, 3);
      pointProgram_.setUniformValue(pointProgram_.uniformLocation("pSize"),
                                    pointSize);
      pointProgram_.setUniformValue(pointProgram_.uniformLocation("vCol"),
                                    vecCol);
      glDrawElements(GL_POINTS, indices_count, GL_UNSIGNED_INT, nullptr);
      pointProgram_.release();
      pointProgram_.removeAllShaders();
    }
    vbo_.release();
    ebo_.release();
    vbo_.destroy();
    ebo_.destroy();
  }
}

Scene::~Scene() {
  context()->doneCurrent();
  vao_.release();
  vao_.destroy();
}

/// @brief (override) Sets shader for line thickness, mode and color
void Scene::compainLineShader() {
  lineProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                       ":shaders/vertex.vert");
  if (line == 0) {
    lineProgram_.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                         ":shaders/line.geom");
    lineProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":shaders/line.frag");
  }
  if (line == 1) {
    lineProgram_.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                         ":shaders/dashLine.geom");
    lineProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":shaders/line.frag");
  }
}

/// @brief (override) Sets shader for point size, mode and color
void Scene::compainPointShader() {
  if (point != 0) {
    pointProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":shaders/vertex.vert");
    if (point == 1) {
      pointProgram_.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                            ":shaders/squarePoint.geom");
    }
    if (point == 2) {
      pointProgram_.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                            ":shaders/circlePoint.geom");
    }
    pointProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":shaders/point.frag");
  }
}
