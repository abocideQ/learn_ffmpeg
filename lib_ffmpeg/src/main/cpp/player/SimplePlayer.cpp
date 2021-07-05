#include "SimplePlayer.h"

extern "C" {

void SimplePlayer::onSource(char *source) {
    if (m_VideoRender == nullptr || m_VideoCodec == nullptr) {
        m_VideoRender = new VideoRender();
        m_VideoCodec = new VideoCodec();
        m_VideoCodec->setRender(m_VideoRender);
    }
    m_VideoCodec->onInit(source);
}

void SimplePlayer::onPlay() {
    m_VideoRender->onResume();
    m_VideoCodec->onResume();
}

void SimplePlayer::onPause() {
    m_VideoCodec->onPause();
}

void SimplePlayer::onStop() {
    m_VideoCodec->onStop();
    m_VideoRender->onStop();
}

void SimplePlayer::onRelease() {
    m_VideoCodec->onStop();
    m_VideoCodec->onRelease();
    m_VideoRender->onRelease();
}

void SimplePlayer::onSurfaceCreated() {
    m_VideoRender->onSurfaceCreated();
}

void SimplePlayer::onSurfaceChanged(int w, int h) {
    m_VideoRender->onSurfaceChanged(w, h);
}

void SimplePlayer::onDrawFrame() {
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







