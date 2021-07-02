#include "SimplePlayer.h"

extern "C" {

void SimplePlayer::onSource(char *source) {
    BaseCodec::instance()->onInit(source, AVMEDIA_TYPE_VIDEO);
//    BaseCodec::instance()->onInit(source, AVMEDIA_TYPE_AUDIO);
}

void SimplePlayer::onPlay() {
    SimpleRender::instance()->onResume();
    BaseCodec::instance()->onResume();
}

void SimplePlayer::onPause() {
    BaseCodec::instance()->onPause();
}

void SimplePlayer::onStop() {
    BaseCodec::instance()->onStop();
    SimpleRender::instance()->onStop();
}

void SimplePlayer::onRelease() {
    BaseCodec::instance()->onStop();
    BaseCodec::instance()->onRelease();
    SimpleRender::instance()->onRelease();
}

void SimplePlayer::onSurfaceCreated() {
    SimpleRender::instance()->onSurfaceCreated();
}

void SimplePlayer::onSurfaceChanged(int w, int h) {
    SimpleRender::instance()->onSurfaceChanged(w, h);
}

void SimplePlayer::onDrawFrame() {
    SimpleRender::instance()->onDrawFrame();
}

SimplePlayer *SimplePlayer::m_Player = nullptr;
SimplePlayer *SimplePlayer::instance() {
    if (m_Player == nullptr) {
        m_Player = new SimplePlayer();
    }
    return m_Player;
}
}







