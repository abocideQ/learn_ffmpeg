#ifndef FFMPEGTEST_BASECODEC_H
#define FFMPEGTEST_BASECODEC_H

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
#include "libavutil/imgutils.h"
#include "libavutil/time.h"
};

#include "SimpleRender.h"
#include "Log.h"

#include <thread>
#include <mutex>

enum DecoderState {
    STATE_UNKNOWN,
    STATE_RESUME,
    STATE_PAUSE,
    STATE_STOP
};

class BaseCodec {
public:
    void onInit(char *url, AVMediaType mediaType);

    void onResume();

    void onPause();

    void onStop();

    void onRelease();

    static BaseCodec *instance();

protected:
    //地址
    char *m_Url = nullptr;
    //媒体类型 AUDIO/VIDEO
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    //打开结果
    int m_Result = -1;

    //解封装Context
    AVFormatContext *m_AVFormatContext = nullptr;
    //编码器
    AVCodec *m_AVCodec = nullptr;
    //编码器Context
    AVCodecContext *m_AVCodecContext = nullptr;
    //编码包
    AVPacket *m_Packet = nullptr;
    //帧数据
    AVFrame *m_Frame = nullptr;
    //音视频流索引
    int m_StreamIndex = 0;

    //视频转换工具Context
    SwsContext *m_SwsContext = nullptr;
    //转换后的帧
    AVFrame *m_FrameScale = nullptr;
    //转换后的数据
    uint8_t *m_FrameScaleBuffer = nullptr;

    //音频采样工具Context
    SwrContext *m_SwrContext = nullptr;
    //音频数据
    uint8_t *m_AudioOutBuffer = nullptr;

    //线程
    std::thread *m_Thread = nullptr;
    volatile int m_Status = STATE_UNKNOWN;
    //互斥锁
    static std::mutex m_Mutex;
private:

    static void codecRunAsy(BaseCodec *ptr);

    void codecInit();

    void codecVideo();

    void swScale();

    void codecAudio();

    void swReSample();

    void synchronization();

    static BaseCodec *m_Sample;
};


#endif //FFMPEGTEST_BASECODEC_H
