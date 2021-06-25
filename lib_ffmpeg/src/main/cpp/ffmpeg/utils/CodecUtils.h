#ifndef FFMPEGTEST_CODECUTILS_H
#define FFMPEGTEST_CODECUTILS_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavcodec/jni.h>
};

class CodecUtils {
public:
    static void codec();

protected:
    AVFormatContext *m_AVFormatContext = nullptr;
    AVCodecContext *m_AVCodecContext = nullptr;
    AVCodec *m_AVCodec = nullptr;
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    char *mUrl = nullptr;
    int m_StreamIndex = 0;
};


#endif //FFMPEGTEST_CODECUTILS_H
