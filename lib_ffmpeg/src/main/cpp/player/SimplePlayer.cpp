#include "SimplePlayer.h"

extern "C" {

void SimplePlayer::onSource(char *source) {
    onStop();
    //video
    if (m_VideoRender == nullptr) {
        m_VideoRender = new VideoRender();
    }
    if (m_VideoDecode == nullptr) {
        m_VideoDecode = new VideoDecode();
    }
    m_VideoDecode->setRender(m_VideoRender);
    m_VideoDecode->onInit(source);
    //audio
    if (m_AudioRender == nullptr) {
        m_AudioRender = new AudioRender();
    }
    if (m_AudioDecode == nullptr) {
        m_AudioDecode = new AudioDecode();
    }
    m_AudioRender->onAudioCreate();
    m_AudioDecode->setRender(m_AudioRender);
    m_AudioDecode->onInit(source);
}

void SimplePlayer::onSeekTo(int percent) {
    if (m_VideoDecode != nullptr && m_VideoRender != nullptr) {
        m_VideoDecode->onSeekTo(percent);
    }
    if (m_AudioDecode != nullptr && m_AudioRender != nullptr) {
        m_AudioDecode->onSeekTo(percent);
    }
}

void SimplePlayer::onPlay() {
    if (m_VideoDecode != nullptr && m_VideoRender != nullptr) {
        m_VideoDecode->onResume();
        m_VideoRender->onResume();
    }
    if (m_AudioDecode != nullptr && m_AudioRender != nullptr) {
        m_AudioDecode->onResume();
        m_AudioRender->onResume();
    }
}

void SimplePlayer::onPause() {
    if (m_VideoDecode != nullptr && m_VideoRender != nullptr) {
        m_VideoDecode->onPause();
        m_VideoRender->onPause();
    }
    if (m_AudioDecode != nullptr && m_AudioRender != nullptr) {
        m_AudioDecode->onPause();
        m_AudioRender->onPause();
    }
}

void SimplePlayer::onStop() {
    if (m_VideoDecode != nullptr) {
        m_VideoDecode->onStop();
        m_VideoDecode->onRelease();
        m_VideoDecode = nullptr;
    }
    if (m_VideoRender != nullptr) {
        m_VideoRender->onStop();
    }
    if (m_AudioDecode != nullptr) {
        m_AudioDecode->onStop();
        m_AudioDecode->onRelease();
        m_AudioDecode = nullptr;
    }
    if (m_AudioRender != nullptr) {
        m_AudioRender->onStop();
        m_AudioRender->onRelease();
        m_AudioRender = nullptr;
    }
}

void SimplePlayer::onRelease() {
    onStop();
    if (m_VideoRender != nullptr) {
        m_VideoRender->onRelease();
        m_VideoRender = nullptr;
    }
    m_Player = nullptr;
}

void SimplePlayer::onSurfaceCreated() {
    if (m_VideoRender == nullptr) return;
    m_VideoRender->onSurfaceCreated();
}

void SimplePlayer::onSurfaceChanged(int w, int h) {
    if (m_VideoRender == nullptr) return;
    m_VideoRender->onSurfaceChanged(w, h);
}

void SimplePlayer::onDrawFrame() {
    if (m_VideoRender == nullptr) return;
    m_VideoRender->onDrawFrame();
}

SimplePlayer *SimplePlayer::m_Player = nullptr;
SimplePlayer *SimplePlayer::instance() {
    if (m_Player == nullptr) {
        m_Player = new SimplePlayer();
    }
    return m_Player;
}
}







