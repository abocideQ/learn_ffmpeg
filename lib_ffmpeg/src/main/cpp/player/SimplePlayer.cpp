#include "SimplePlayer.h"

extern "C" {

void SimplePlayer::onSource(char *source) {
    BaseCodec::instance()->onCodecInit(source, AVMEDIA_TYPE_VIDEO);
}

void SimplePlayer::onPlay() {
}

void SimplePlayer::onPause() {

}

void SimplePlayer::onRelease() {

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







