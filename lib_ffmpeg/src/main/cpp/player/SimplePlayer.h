#ifndef FFMPEGTEST_SIMPLEPLAYER_H
#define FFMPEGTEST_SIMPLEPLAYER_H

#include "BaseCodec.h"
#include "Log.h"

class SimplePlayer {
public:
    void onSource(char *source);

    void onPlay();

    void onPause();

    void onStop();

    void onRelease();

    void onSurfaceCreated();

    void onSurfaceChanged(int w, int h);

    void onDrawFrame();

    static SimplePlayer *instance();

private:
    static SimplePlayer *m_Player;
};


#endif //FFMPEGTEST_SIMPLEPLAYER_H
