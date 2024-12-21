#include <QApplication>
#include <QOpenGLContext>

#include "video_player.h"
#include <QMainWindow>

int main(int argc, char *argv[]) {
  // QGuiApplication app(argc, argv);
  QApplication a(argc, argv);
  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(32);
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
    qDebug("Requesting 2.1 core context");
    fmt.setVersion(2, 1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
  } else {
    qDebug("Requesting 3.0 context");
    fmt.setVersion(3, 0);
  }

  QSurfaceFormat::setDefaultFormat(fmt);

  QMainWindow w;
  VideoPlayerWidget *videoWidget = new VideoPlayerWidget;
  w.setCentralWidget(videoWidget);
  w.resize(700, 500);
  w.show();

  return a.exec();
}
