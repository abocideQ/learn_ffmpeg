#include "AudioRender.h"

extern "C" {

void AudioRender::onBuffer(uint8_t *data, int size) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    if (m_Frame != nullptr && m_Frame->data != nullptr) {
        free(m_Frame->data);
        m_Frame = nullptr;
    }
    m_Frame = new AudioFrame(data, size);
}

void AudioRender::onAudioCreate() {
    SLUtils::slProgramCreate(m_EngineObj, m_EngineItf, m_MixerObj, m_MixerItf, m_PlayerObj,
                             m_PlayerItf, m_VolumeItf, m_BufferQueueItf);
    SLUtils::slCallbackRegister(m_BufferQueueItf, AudioPlayerCallback, this)
    m_Thread = new std::Thread(audioRunAsy, this);
}

void AudioRender::audioRunAsy(AudioRender *ptr) {
    ptr->onDrawFrame();
}

void AudioRender::onDrawFrame() {
    (*m_PlayerItf)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_PLAYING);
    for (;;) {
        while (m_Interrupt == -1) {
            continue;
        }
        if (m_Interrupt == 1) {
            break;
        }
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (m_PlayerItf && m_Frame != nullptr) {
            SLresult result = (*m_BufferQueueItf)->Enqueue(m_BufferQueueItf, m_Frame->data,
                                                           (SLuint32) m_Frame->dataSize);
            if (result == SL_RESULT_SUCCESS) {
                if (m_Frame != nullptr && m_Frame->data != nullptr) {
                    free(m_Frame->data);
                    m_Frame = nullptr;
                }
            } else {
                LOGCATE("AudioRender: Enqueue error =%d", result);
            }
        }
        lock.unlock();
    }
}

void AudioRender::onResume() {
    m_Interrupt = 0;
    (*m_PlayerItf)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_PLAYING);
}

void AudioRender::onPause() {
    m_Interrupt = -1;
    (*m_PlayerItf)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_PAUSED);
}

void AudioRender::onStop() {
    m_Interrupt = 1;
    (*m_PlayerItf)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_STOPPED);
}

void AudioRender::onRelease() {
    m_Interrupt = 1;
    (*m_PlayerItf)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_STOPPED);

}

void AudioRender::handlerCallBack(SLAndroidSimpleBufferQueueItf bufferQueue) {
}

void AudioRender::AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
    AudioRender *instance = static_cast<AudioRender *>(context);
    instance->handlerCallBack(bufferQueue);
}

}


