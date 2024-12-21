HEADERS = \
          video_player.h \
          video_reader.h

SOURCES = \
          main.cpp \
          video_reader.cpp

RESOURCES += hellogles3.qrc
QT        += core gui widgets

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libavcodec libavdevice libavfilter libavformat libavutil libpostproc libswresample libswscale
