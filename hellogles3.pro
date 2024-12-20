HEADERS = glwindow.h \
          logo.h \
          cameraopenglwidget.h \
          video_player.h \
          video_reader.h

SOURCES = glwindow.cpp \
          main.cpp \
          logo.cpp \
          video_reader.cpp

RESOURCES += hellogles3.qrc
QT        += core gui widgets

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libavcodec libavdevice libavfilter libavformat libavutil libpostproc libswresample libswscale
