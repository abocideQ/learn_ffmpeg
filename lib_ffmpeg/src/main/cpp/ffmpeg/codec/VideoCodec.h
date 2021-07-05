#ifndef FFMPEGTEST_VIDEOCODEC_H
#define FFMPEGTEST_VIDEOCODEC_H

#include "BaseCodec.h"
#include "VideoRender.h"

class VideoCodec : public BaseCodec {

public:
    void setRender(VideoRender *render);

    void onInit(char *url);

    void onResume();

    void onPause();

    void onStop();

    void onRelease();

protected:
    VideoRender *m_Render = nullptr;

    void codecHandler(AVFrame *frame);
};


#endif //FFMPEGTEST_VIDEOCODEC_H
