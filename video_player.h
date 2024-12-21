#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H
#include "video_reader.h"
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QTimer>
#include <QtDebug>
class VideoPlayerWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  VideoPlayerWidget(QWidget *parent = nullptr)
      : QOpenGLWidget(parent), texture(0), frameData(nullptr) {

    //        if (!video_reader_open(&vr_state, "/dev/video0")) {
    //                printf("Couldn't open video device\n");
    //                return;
    //        }

    if (!video_reader_open_file(&vr_state,
                                "/home/zhang/Downloads/big_buck_bunny.mp4")) {
      printf("Couldn't open video device\n");
      return;
    }
    // 为视频帧数据分配内存
    frame_width = vr_state.width;
    frame_height = vr_state.height;
    frameData = new uint8_t[frame_width * frame_height * 4];
    // 初始化定时器，每秒30帧
    frameUpdateTimer = new QTimer(this);
    connect(frameUpdateTimer, &QTimer::timeout, this,
            &VideoPlayerWidget::updateFrame);
    frameUpdateTimer->start(1000 / 30); // 每秒30帧更新
  }

  ~VideoPlayerWidget() {
    // 清理资源
    if (frameData) {
      delete[] frameData;
    }
  }

protected:
  void initializeGL() override {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置背景色
    // 设置纹理过滤和环绕方式
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  void resizeGL(int w, int h) override {
    glViewport(0, 0, w, h); // 设置视口大小为窗口的宽度和高度

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    //    GLenum err;
    //    int i = 0;
    //    while (((err = glGetError()) != GL_NO_ERROR) && (i++ < 10)) {
    //      qDebug() << "OpenGL error: " << err;
    //    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // 设置模型视图矩阵
  }

  void paintGL() override {
    //    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT); // 清空画布

    if (texture) {
      glBindTexture(GL_TEXTURE_2D, texture);

      glBegin(GL_QUADS);
      glTexCoord2d(0, 0);
      glVertex2i(0, 0);
      glTexCoord2d(1, 0);
      glVertex2i(frame_width, 0);
      glTexCoord2d(1, 1);
      glVertex2i(frame_width, frame_height);
      glTexCoord2d(0, 1);
      glVertex2i(0, frame_height);
      glEnd();
    }
  }

public slots:
  void updateFrame() {

    // 调用读取视频帧数据的函数
    int64_t pts;
    if (video_reader_read_frame(&vr_state, frameData, &pts)) {
      // 将读取到的原始图像数据转换为 QImage
      // 创建纹理
      //      if (texture) {
      //        glDeleteTextures(1, &texture); // 释放旧的纹理
      //      }
      //      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);

      // 使用 glTexImage2D 来加载纹理，使用 RGBA 格式的原始数据
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame_width, frame_height, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, frameData);

      // 更新界面
      update(); // 调用 paintGL() 绘制图像
    }
  }

private:
  int frame_width, frame_height;
  VideoReaderState vr_state{};
  GLuint texture; // OpenGL 纹理句柄
  uint8_t *frameData;
  QTimer *frameUpdateTimer;
};

#endif // VIDEO_PLAYER_H
