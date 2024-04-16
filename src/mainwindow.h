#ifndef CPP4_3DVIEWER_SRC_MAINWINDOW_H_
#define CPP4_3DVIEWER_SRC_MAINWINDOW_H_

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QMainWindow>
#include <QPaintEvent>
#include <QPixmap>
#include <QSettings>
#include <QSurfaceFormat>

#include "./controller/controller.h"
#include "QtGui/qsurfaceformat.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void SetController(s21::Controller *controller);

 private:
  void paintEvent(QPaintEvent *event) override;
  void SaveSettings();
  void LoadSettings();
  void Timeout();

  Ui::MainWindow *ui;
  std::vector<float> *vertices_;
  std::vector<unsigned int> *faces_;
  s21::Controller *controller_;
  QSettings *settings_;
  QSurfaceFormat window_surface_;
  QList<QImage> gif_frames_;
  bool recording_;

 private slots:
  void SetLineThickness();
  void SetDotSize();
  void SetLineMode();
  void SetDotMode();
  void SetPerspectMode();
  void SetLineColor();
  void SetPointColor();
  void SetBGColor();
  void GetFilename();
  void RedrawObj();
  void TakeScreenshot();
  void RecordGif();
  void RenderGif();
};
#endif  // CPP4_3DVIEWER_SRC_MAINWINDOW_H_
