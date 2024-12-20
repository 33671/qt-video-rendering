#include "video_reader.h"
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWindow>
#define FRAME_RATE 30
class CamWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  CamWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent), m_context(0) {

    // vr_state.sws_scaler_ctx = NULL;
    //        if (!video_reader_open(&vr_state, "/dev/video0")) {
    //                printf("Couldn't open video device\n");
    //                return;
    //        }

    if (!video_reader_open_file(&vr_state, "/home/zhang/Desktop/test.mp4")) {
      printf("Couldn't open video device\n");
      return;
    }
    // qDebug() << "here";
    frame_width = vr_state.width;
    frame_height = vr_state.height;
    frame_data = new uint8_t[frame_width * frame_height * 4];

    QTimer *frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &CamWidget::updateFrame);
    //    frameTimer->start(1000 /
    //                      FRAME_RATE); // Set frameRate as required (e.g., 30
    //                      FPS)
  }

  ~CamWidget() {
    makeCurrent();
    //        delete m_texture;
    //        delete m_program;
    //        delete m_vbo;
    //        delete m_vao;
  }

protected:
  void initializeGL() override {
    initializeOpenGLFunctions();

    // Set up OpenGL textures and shaders here
    glGenTextures(1, &m_texHandle);
    glBindTexture(GL_TEXTURE_2D, m_texHandle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    //        GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D,
    //        GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    qDebug() << "init";
    //        QOpenGLFramebufferObjectFormat format;
    //        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    //        fbo = new QOpenGLFramebufferObject(frame_width , frame_height,
    //        format);

    // Other initialization...
    //        if(fbo->bind())
    //        {
    //            qDebug() << "double buffer enabled";
    //        }
  }

  void resizeGL(int w, int h) override {
    qDebug() << "resize triggered" << w << h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
  }

  void paintGL() override {
    // Bind the framebuffer to render to the texture
    // fbo->bind();
    // Debug() << 2;
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 清空屏幕的颜色缓冲区,填充glClearColor设置的颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int64_t pts;
    if (!video_reader_read_frame(&vr_state, frame_data, &pts)) {
      printf("Couldn't load video frame\n");
      return;
    }
    qDebug() << frame_width << frame_height;
    //绑定纹理并绘制
    glBindTexture(GL_TEXTURE_2D, m_texHandle);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0,
    //                 GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texHandle);
    glTexCoord2d(0, 0);
    glVertex2i(0, 0);
    glTexCoord2d(1, 0);
    glVertex2i(frame_width, 0);
    glTexCoord2d(1, 1);
    glVertex2i(frame_width, frame_height);
    glTexCoord2d(0, 1);
    glVertex2i(0, frame_height);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // fbo->release();
    // update();
  }
  void updateFrame() {
    int64_t pts;
    //    if (!video_reader_read_frame(&vr_state, frame_data, &pts)) {
    //      printf("Couldn't load video frame\n");
    //      return;
    //    }
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0,
    //                 GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
    //    if (!m_context) {
    //      m_context = new QOpenGLContext(this);
    //      m_context->setFormat(requestedFormat());
    //      m_context->create();

    //      needsInitialize = true;
    //    }

    //    m_context->makeCurrent(this);

    // Update the OpenGL texture with the new frame
    //    glBindTexture(GL_TEXTURE_2D, m_texHandle);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0,
    //                 GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
    // update(); // Trigger a paintGL() call
  }

private:
  int frame_width = 640;
  int frame_height = 480;
  QOpenGLFramebufferObject *fbo;
  uint8_t *frame_data;
  VideoReaderState vr_state{};
  GLuint m_texHandle;
  QOpenGLTexture *m_texture;
  QOpenGLShaderProgram *m_program;
  QOpenGLBuffer *m_vbo;
  QOpenGLVertexArrayObject *m_vao;
  QOpenGLContext *m_context;
};
