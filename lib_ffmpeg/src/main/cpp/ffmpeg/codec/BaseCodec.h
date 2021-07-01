#ifndef FFMPEGTEST_BASECODEC_H
#define FFMPEGTEST_BASECODEC_H

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
#include "libavutil/time.h"
};

#include "SimpleRender.h"
#include "Log.h"

#include <thread>
#include <mutex>

class BaseCodec {
public:

    static void onRunAsy(BaseCodec *ptr);

    void onCodecInit(char *url, AVMediaType mediaType);

    void onDestroy();

    static BaseCodec *instance();

protected:
    //视频地址
    char *m_Url = nullptr;
    //媒体类型 AUDIO/VIDEO
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    //解析结果
    int m_Result = -1;

    //FormatContext
    AVFormatContext *m_AVFormatContext = nullptr;
    //CodecContext
    AVCodecContext *m_AVCodecContext = nullptr;
    //Codec
    AVCodec *m_AVCodec = nullptr;
    //解包
    AVPacket *m_Packet = nullptr;
    //帧
    AVFrame *m_Frame = nullptr;
    //索引
    int m_StreamIndex = 0;

    //scaleContext
    SwsContext *m_SwsContext = nullptr;
    //转换后的帧
    AVFrame *m_FrameScale = nullptr;
    //转换后的数据
    uint8_t *m_FrameScaleBuffer = nullptr;

    //线程
    std::thread *m_Thread = nullptr;
    //锁
    static std::mutex m_Mutex;
private:
    void initCodecContext();

    void videoCodec();

    void swScale();

    void audioCodec();

    void swReSample();

    static BaseCodec *m_Sample;
};


#endif //FFMPEGTEST_BASECODEC_H
