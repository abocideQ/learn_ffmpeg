#include "SimplePlayer.h"

extern "C" {

void SimplePlayer::onSource(char *source) {
    onStop();
    //video
    if (m_VideoRender == nullptr) {
        m_VideoRender = new VideoRender();
    }
    if (m_VideoCodec == nullptr) {
        m_VideoCodec = new VideoCodec();
    }
    m_VideoCodec->setRender(m_VideoRender);
    m_VideoCodec->onInit(source);
    //audio
    if (m_AudioRender == nullptr) {
        m_AudioRender = new AudioRender();
    }
    if (m_AudioCodec == nullptr) {
        m_AudioCodec = new AudioCodec();
    }
    m_AudioRender->onAudioCreate();
    m_AudioCodec->setRender(m_AudioRender);
    m_AudioCodec->onInit(source);
}

void SimplePlayer::onSeekTo(int percent) {
    if (m_VideoCodec != nullptr && m_VideoRender != nullptr) {
        m_VideoCodec->onSeekTo(percent);
    }
    if (m_AudioCodec != nullptr && m_AudioRender != nullptr) {
        m_AudioCodec->onSeekTo(percent);
    }
}

void SimplePlayer::onPlay() {
    if (m_VideoCodec != nullptr && m_VideoRender != nullptr) {
        m_VideoCodec->onResume();
        m_VideoRender->onResume();
    }
    if (m_AudioCodec != nullptr && m_AudioRender != nullptr) {
        m_AudioCodec->onResume();
        m_AudioRender->onResume();
    }
}

void SimplePlayer::onPause() {
    if (m_VideoCodec != nullptr && m_VideoRender != nullptr) {
        m_VideoCodec->onPause();
        m_VideoRender->onPause();
    }
    if (m_AudioCodec != nullptr && m_AudioRender != nullptr) {
        m_AudioCodec->onPause();
        m_AudioRender->onPause();
    }
}

void SimplePlayer::onStop() {
    if (m_VideoCodec != nullptr) {
        m_VideoCodec->onStop();
        m_VideoCodec->onRelease();
        m_VideoCodec = nullptr;
    }
    if (m_VideoRender != nullptr) {
        m_VideoRender->onStop();
    }
    if (m_AudioCodec != nullptr) {
        m_AudioCodec->onStop();
        m_AudioCodec->onRelease();
        m_AudioCodec = nullptr;
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







