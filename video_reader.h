#ifndef VIDEO_READER_H
#define VIDEO_READER_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <inttypes.h>
}

struct VideoReaderState {
    // Public things for other parts of the program to read from
    int width, height;
    AVRational time_base;

    // Private internal state
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    int video_stream_index;
    AVFrame* av_frame;
    AVPacket* av_packet;
    SwsContext* sws_scaler_ctx;
};

bool video_reader_open(VideoReaderState* state, const char* device_name);
bool video_reader_open_file(VideoReaderState* state, const char* file_name);
bool video_reader_read_frame(VideoReaderState* state, uint8_t* frame_buffer, int64_t* pts);
bool video_reader_seek_frame(VideoReaderState* state, int64_t ts);
void video_reader_close(VideoReaderState* state);


#endif // VIDEO_READER_H
