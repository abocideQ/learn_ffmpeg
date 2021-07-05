#ifndef FFMPEGTEST_AUDIOCODEC_H
#define FFMPEGTEST_AUDIOCODEC_H

#include "BaseCodec.h"

class AudioCodec : public BaseCodec {
public:
    void onInit(char *url);

    void onResume();

    void onPause();

    void onStop();

    void onRelease();

protected:
    void codecHandler(AVFrame *frame);
};


#endif //FFMPEGTEST_AUDIOCODEC_H
