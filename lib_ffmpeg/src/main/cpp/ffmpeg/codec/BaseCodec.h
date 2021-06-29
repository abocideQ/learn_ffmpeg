#ifndef FFMPEGTEST_BASECODEC_H
#define FFMPEGTEST_BASECODEC_H

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
#include "PixImage.h"
};

class BaseCodec {
public:
    void init(char *url, AVMediaType mediaType);

    void videoCodec();

    void audioCodec();

    void destroy();

protected:
    char *m_Url = nullptr;
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    int m_Result = -1;

    AVFormatContext *m_AVFormatContext = nullptr;
    AVCodecContext *m_AVCodecContext = nullptr;
    AVCodec *m_AVCodec = nullptr;
    AVPacket *m_Packet = nullptr;
    AVFrame *m_Frame = nullptr;
    int m_StreamIndex = 0;

    SwsContext *m_SwsContext = nullptr;
    AVFrame *m_FrameScale = nullptr;
    uint8_t *m_FrameScaleBuffer = nullptr;
private:
    void swScale();

    void swReSample();
};


#endif //FFMPEGTEST_BASECODEC_H
