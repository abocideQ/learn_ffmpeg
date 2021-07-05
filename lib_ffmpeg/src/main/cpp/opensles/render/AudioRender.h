#ifndef FFMPEGTEST_AUDIORENDER_H
#define FFMPEGTEST_AUDIORENDER_H

#include "SLUtils.h"
#include "PixAudio.h

#include <thread>
#include <mutex>

class AudioRender {
public:
    void onBuffer(uint8_t *data, int size);

    void onAudioCreate();

    void onDrawFrame();

    void onResume();

    void onPause();

    void onStop();

    void onRelease();

protected:
    //SL引擎
    SLObjectItf m_EngineObj;
    SLEngineItf m_EngineItf;
    //SL输出
    SLObjectItf m_MixerObj;
    SLOutputMixItf m_MixerItf;
    //SLPlayer
    SLObjectItf m_PlayerObj;
    SLPlayItf m_PlayerItf;
    //SL音量
    SLVolumeItf m_VolumeItf;
    //SLBuffer
    SLAndroidSimpleBufferQueueItf m_BufferQueueItf;

    //data
    AudioFrame *m_Frame;
    //线程
    std::thread *m_Thread = nullptr;
    volatile int m_Interrupt = 0;
    //互斥锁
    static std::mutex m_Mutex;
private:
    static void audioRunAsy(AudioRender *ptr);

    static void AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context);

    void handlerCallBack(SLAndroidSimpleBufferQueueItf bufferQueue);
};


#endif //FFMPEGTEST_AUDIORENDER_H
