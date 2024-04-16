/** @mainpage 3DViewer 2.0
 *
 * @authors hotblack, mckinley
 *
 * An app to display 3D .obj models. Yeah, this one
 *
 */
#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  s21::Controller controller;
  w.SetController(&controller);
  w.show();
  return a.exec();
}
