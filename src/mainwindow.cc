/// @file mainwindow.cc
#include "mainwindow.h"

#include <gif/gif.h>

#include <QPalette>
#include <chrono>
#include <iostream>
#include <thread>

#include "./ui_mainwindow.h"
/// @brief MainWindow cnstructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), recording_(false) {
  /*  Это магический кусок кода. Без него шейдеры работать отказываются. Не
   * удалять */
  window_surface_ = QSurfaceFormat::defaultFormat();
  window_surface_.setDepthBufferSize(24);
  window_surface_.setMajorVersion(4);
  window_surface_.setMinorVersion(3);
  window_surface_.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(window_surface_);
  /* Конец магического куска кода */

  ui->setupUi(this);
  LoadSettings();
  connect(ui->button_apply, SIGNAL(clicked()), this, SLOT(RedrawObj()));
  connect(ui->button_open, SIGNAL(clicked()), this, SLOT(GetFilename()));
  connect(ui->button_line_color, SIGNAL(clicked()), this, SLOT(SetLineColor()));
  connect(ui->button_point_color, SIGNAL(clicked()), this,
          SLOT(SetPointColor()));
  connect(ui->button_bg_color, SIGNAL(clicked()), this, SLOT(SetBGColor()));

  connect(ui->radio_point_circle, SIGNAL(clicked()), this, SLOT(SetDotMode()));
  connect(ui->radio_point_none, SIGNAL(clicked()), this, SLOT(SetDotMode()));
  connect(ui->radio_point_square, SIGNAL(clicked()), this, SLOT(SetDotMode()));
  connect(ui->radio_solid, SIGNAL(clicked()), this, SLOT(SetLineMode()));
  connect(ui->radio_dashed, SIGNAL(clicked()), this, SLOT(SetLineMode()));
  connect(ui->radi_projection_central, SIGNAL(clicked()), this,
          SLOT(SetPerspectMode()));
  connect(ui->radi_projection_parallel, SIGNAL(clicked()), this,
          SLOT(SetPerspectMode()));
  connect(ui->slider_line_thickness, SIGNAL(sliderReleased()), this,
          SLOT(SetLineThickness()));
  connect(ui->slider_point_size, SIGNAL(sliderReleased()), this,
          SLOT(SetDotSize()));
  connect(ui->button_screenshot, SIGNAL(clicked()), this,
          SLOT(TakeScreenshot()));
  connect(ui->button_gif, SIGNAL(clicked()), this, SLOT(RecordGif()));
  connect(ui->button_gif, SIGNAL(released()), this, SLOT(RenderGif()));
  ui->widget->update();
}

/// @brief Connects controller instance to window
void MainWindow::SetController(s21::Controller *controller) {
  controller_ = controller;
  vertices_ = &controller_->GetVertices();
  faces_ = &controller_->GetFaces();
};

/// @brief Reads line thickness setting and sends it to widget to proceed
void MainWindow::SetLineThickness() {
  if (ui->slider_line_thickness->value() == 0) {
    ui->button_line_color->setDisabled(true);
  } else {
    ui->button_line_color->setDisabled(false);
  }
  ui->widget->lineWidth = 0.0045 * ui->slider_line_thickness->value();
  ui->widget->update();
}

/// @brief Reads dot size setting and sends it to widget to proceed
void MainWindow::SetDotSize() {
  if (ui->slider_point_size->value() == 0 ||
      ui->radio_point_none->isChecked()) {
    ui->button_point_color->setDisabled(true);
  } else {
    ui->button_point_color->setDisabled(false);
  }
  ui->widget->pointSize = 0.0045 * ui->slider_point_size->value();
  ui->widget->update();
}

/// @brief Reads line mode setting (dashed or solid) abd sends it to widget to
/// proceed
void MainWindow::SetLineMode() {
  if (ui->radio_solid->isChecked()) {
    ui->widget->line = 0;
  } else if (ui->radio_dashed->isChecked()) {
    ui->widget->line = 1;
  }
  ui->widget->update();
}

/// @brief Reads perspective mode and sends it to widget to proceed
void MainWindow::SetPerspectMode() {
  if (ui->radi_projection_central->isChecked()) {
    ui->widget->perspective = 0;
  } else if (ui->radi_projection_parallel->isChecked()) {
    ui->widget->perspective = 1;
  }
  ui->widget->update();
}

/// @brief Reads dot figure setting and sends it to widget to proceed
void MainWindow::SetDotMode() {
  if (ui->radio_point_none->isChecked()) {
    ui->widget->point = 0;
    ui->button_point_color->setDisabled(true);
  } else if (ui->radio_point_square->isChecked()) {
    ui->widget->point = 1;
    ui->button_point_color->setDisabled(false);
  } else if (ui->radio_point_circle->isChecked()) {
    ui->widget->point = 2;
    ui->button_point_color->setDisabled(false);
  }
  ui->widget->update();
}

/// @brief Reads point color setting (using color dialog window) and sends it to
/// widget to proceed
void MainWindow::SetPointColor() {
  QColorDialog dialog;
  QColor color = dialog.getColor();
  if (color.isValid()) {
    ui->widget->vecCol = color;
    ui->widget->update();
  }
}

/// @brief Reads background color setting (using color dialog window) and sends
/// it to widget to proceed
void MainWindow::SetBGColor() {
  QColorDialog dialog;
  QColor color = dialog.getColor();
  if (color.isValid()) {
    ui->widget->bgCol = color;
    ui->widget->repaint();
    ui->widget->update();
  }
}

/// @brief Reads line color setting (using color dialog window) and sends it to
/// widget to proceed
void MainWindow::SetLineColor() {
  QColorDialog dialog;
  QColor color = dialog.getColor();
  if (color.isValid()) {
    ui->widget->lineCol = color;
    ui->widget->update();
  }
}

/// @brief Sends all transfrmation data over to controller
void MainWindow::RedrawObj() {
  controller_->MoveObj((float)ui->spinbox_move_x->value(),
                       (float)ui->spinbox_move_y->value(),
                       (float)ui->spinbox_move_z->value());
  controller_->RotateObj((float)ui->spinbox_rotate_x->value(),
                         (float)ui->spinbox_rotate_y->value(),
                         (float)ui->spinbox_rotate_z->value());
  controller_->ScaleObj((float)ui->zoom_spinbox->value());
  ui->widget->update();
}

/// @brief Reads name of obj file and sends it to controller
void MainWindow::GetFilename() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/",
                                                  tr("OBJ files (*.obj)"));
  try {
    controller_->OpenFile(filename.toStdString());
    ui->lineedit_file_input->setText(filename);
    ui->widget->vertices_ = vertices_;
    ui->widget->indices_ = faces_;
    ui->widget->indices_count = faces_->size();
    ui->widget->vertices_count = vertices_->size();
    ui->vertex_amount->setText(QString::number(vertices_->size() / 3));
    ui->edge_amount->setText(QString::number(faces_->size() / 3));
  } catch (std::exception &e) {
    ui->lineedit_file_input->setText(e.what());
  }
  ui->widget->update();
}

/// @brief Takes a picture of a widget
void MainWindow::TakeScreenshot() {
  const QString format = "png";
  QPixmap pixmap(ui->widget->width(), ui->widget->height());
  QFileDialog file_dialog(this, tr("Save as"), "/");
  file_dialog.setAcceptMode(QFileDialog::AcceptSave);
  file_dialog.setFileMode(QFileDialog::AnyFile);
  file_dialog.setDirectory("/");
  file_dialog.setMimeTypeFilters({"image/jpeg", "image/png"});
  file_dialog.selectMimeTypeFilter("image/" + format);
  file_dialog.setDefaultSuffix(format);
  if (file_dialog.exec() != QDialog::Accepted) return;
  const QString fileName = file_dialog.selectedFiles().at(0);
  ui->widget->render(&pixmap);
  if (!pixmap.save(fileName)) {
    ui->label_file_info->setText("couldn't save image");
  }
}

/// @brief Saves current display info into setings file
void MainWindow::SaveSettings() {
  settings_ = new QSettings("S21", "Viewer");
  settings_->beginGroup("Point");
  settings_->setValue("PointType", ui->widget->point);
  settings_->setValue("PointSize", ui->widget->pointSize);
  settings_->setValue("SlidePoint", ui->slider_point_size->value());
  settings_->setValue("PointColorRed", ui->widget->vecCol.red());
  settings_->setValue("PointColorGreen", ui->widget->vecCol.green());
  settings_->setValue("PointColorBlue", ui->widget->vecCol.blue());
  settings_->endGroup();

  settings_->beginGroup("Line");
  settings_->setValue("LineType", ui->widget->line);
  settings_->setValue("LineWidth", ui->widget->lineWidth);
  settings_->setValue("SlideLine", ui->slider_line_thickness->value());
  settings_->setValue("LineColorRed", ui->widget->lineCol.red());
  settings_->setValue("LineColorGreen", ui->widget->lineCol.green());
  settings_->setValue("LineColorBlue", ui->widget->lineCol.blue());
  settings_->endGroup();

  settings_->beginGroup("BackAndPerspective");
  settings_->setValue("BgColorRed", ui->widget->bgCol.red());
  settings_->setValue("BgColorGreen", ui->widget->bgCol.green());
  settings_->setValue("BgColorBlue", ui->widget->bgCol.blue());
  settings_->setValue("Perspective", ui->widget->perspective);
  settings_->endGroup();
}

/// @brief Loads setting on app opening (with interface view)
void MainWindow::LoadSettings() {
  settings_ = new QSettings("S21", "Viewer");
  settings_->beginGroup("Point");
  auto tmpVal = settings_->value("PointType");
  if (tmpVal == 0) {
    ui->widget->point = tmpVal.toInt();
    ui->radio_point_none->setChecked(true);
  } else if (tmpVal == 1) {
    ui->widget->point = tmpVal.toInt();
    ui->radio_point_square->setChecked(true);
  } else if (tmpVal == 2) {
    ui->widget->point = tmpVal.toInt();
    ui->radio_point_circle->setChecked(true);
  }
  tmpVal = settings_->value("PointSize", 0.0045f);
  ui->widget->pointSize = tmpVal.toFloat();
  tmpVal = settings_->value("SlidePoint");
  ui->slider_point_size->setSliderPosition(tmpVal.toInt());
  tmpVal = settings_->value("PointColorRed", 255);
  ui->widget->vecCol.setRed(tmpVal.toInt());
  tmpVal = settings_->value("PointColorGreen");
  ui->widget->vecCol.setGreen(tmpVal.toInt());
  tmpVal = settings_->value("PointColorBlue");
  ui->widget->vecCol.setBlue(tmpVal.toInt());
  settings_->endGroup();

  settings_->beginGroup("Line");
  tmpVal = settings_->value("LineType");
  if (tmpVal == 0) {
    ui->widget->line = tmpVal.toInt();
    ui->radio_solid->setChecked(true);
  } else if (tmpVal == 1) {
    ui->widget->line = tmpVal.toInt();
    ui->radio_dashed->setChecked(true);
  }
  tmpVal = settings_->value("LineSize", 0.0045f);
  ui->widget->lineWidth = tmpVal.toFloat();
  tmpVal = settings_->value("SlideLine");
  ui->slider_line_thickness->setSliderPosition(tmpVal.toInt());
  tmpVal = settings_->value("LineColorRed");
  ui->widget->lineCol.setRed(tmpVal.toInt());
  tmpVal = settings_->value("LineColorGreen");
  ui->widget->lineCol.setGreen(tmpVal.toInt());
  tmpVal = settings_->value("LineColorBlue", 255);
  ui->widget->lineCol.setBlue(tmpVal.toInt());
  settings_->endGroup();

  settings_->beginGroup("BackAndPerspective");
  tmpVal = settings_->value("BgColorRed");
  ui->widget->bgCol.setRed(tmpVal.toInt());
  tmpVal = settings_->value("BgColorGreen");
  ui->widget->bgCol.setGreen(tmpVal.toInt());
  tmpVal = settings_->value("BgColorBlue");
  ui->widget->bgCol.setBlue(tmpVal.toInt());
  tmpVal = settings_->value("Perspective");
  if (tmpVal == 0) {
    ui->widget->perspective = tmpVal.toInt();
    ui->radi_projection_central->setChecked(true);
  } else if (tmpVal == 1) {
    ui->widget->perspective = tmpVal.toInt();
    ui->radi_projection_parallel->setChecked(true);
  }
  settings_->endGroup();
  settings_->clear();
}

/// @brief Starts a GIF recording process
void MainWindow::RecordGif() {
  std::thread timeout(&MainWindow::Timeout, this);
  timeout.detach();
}

/// @brief Service fuunction. Sets a 5-second timeout to record a gif in that
/// time
void MainWindow::Timeout() {
  ui->button_gif->setDisabled(true);
  ui->label_file_info->setText("GIF recording");
  recording_ = true;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  ui->button_gif->setEnabled(true);
  emit ui->button_gif->released();
  recording_ = false;
  ui->label_file_info->setText("");
}

/// @brief Actual GIF file processor. Connects all GIF screenshot into one file
void MainWindow::RenderGif() {
  if (recording_) {
    GifWriter gif;
    QFileDialog file_dialog(this, tr("Save as"), "/", "GIF (*.gif)");
    file_dialog.setAcceptMode(QFileDialog::AcceptSave);
    file_dialog.setFileMode(QFileDialog::AnyFile);
    file_dialog.setDirectory("/");
    file_dialog.setDefaultSuffix("gif");
    if (file_dialog.exec() != QDialog::Accepted) return;
    const QString filename = file_dialog.selectedFiles().at(0);
    if (gif_frames_.size()) {
      auto frame = gif_frames_.begin();
      GifBegin(&gif, filename.toStdString().c_str(), frame->width(),
               frame->height(), 10);
      for (; frame != gif_frames_.end(); frame++) {
        GifWriteFrame(
            &gif, frame->convertToFormat(QImage::Format_RGBA8888).constBits(),
            frame->width(), frame->height(), 10);
      }
      GifEnd(&gif);
    }
    gif_frames_.clear();
  }
}

//// @brief Override of standart paint event. Needed to take snapshots for GIF
void MainWindow::paintEvent(QPaintEvent *event) {
  QMainWindow::paintEvent(event);
  if (recording_) {
    gif_frames_.push_back(ui->widget->grabFramebuffer().scaled(
        QSize(480, 640), Qt::KeepAspectRatio));
  }
}

/// @brief Destructor. Saves settings on close
MainWindow::~MainWindow() {
  SaveSettings();
  delete ui;
}
