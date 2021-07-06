#ifndef FFMPEGTEST_SIMPLEPLAYER_H
#define FFMPEGTEST_SIMPLEPLAYER_H

#include "VideoCodec.h"
#include "AudioCodec.h"

class SimplePlayer {
public:
    void onSource(char *source);

    void onSeekTo(int percent);

    void onPlay();

    void onPause();

    void onStop();

    void onRelease();

    void onSurfaceCreated();

    void onSurfaceChanged(int w, int h);

    void onDrawFrame();

    static SimplePlayer *instance();

protected:
    VideoRender *m_VideoRender = nullptr;

    VideoCodec *m_VideoCodec = nullptr;

    AudioRender *m_AudioRender = nullptr;

    AudioCodec *m_AudioCodec = nullptr;

private:
    static SimplePlayer *m_Player;

};


#endif //FFMPEGTEST_SIMPLEPLAYER_H
