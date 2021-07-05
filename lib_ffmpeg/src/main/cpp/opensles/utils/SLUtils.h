#ifndef FFMPEGTEST_SLUTILS_H
#define FFMPEGTEST_SLUTILS_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "Log.h"

class SLUtils {
public:
    static int slProgramCreate(SLObjectItf engineObj, SLEngineItf engineItf,
                               SLObjectItf mixerObj, SLOutputMixItf mixerItf,
                               SLObjectItf playerObj, SLPlayItf playerItf,
                               SLVolumeItf volumeItf, SLAndroidSimpleBufferQueueItf bufferQueueItf);

    static int slCallbackRegister(SLAndroidSimpleBufferQueueItf bufferQueueItf,
                                  slAndroidSimpleBufferQueueCallback call, void *pContext);

protected:

    static int createEngine(SLObjectItf engineObj, SLEngineItf engineItf);

    static int createMixer(SLEngineItf engineItf, SLObjectItf mixerObj, SLOutputMixItf mixerItf);

    static int createPlayer(SLEngineItf engineItf,
                            SLObjectItf playerObj, SLPlayItf playerItf, SLObjectItf mixerObj,
                            SLVolumeItf volumeItf, SLAndroidSimpleBufferQueueItf bufferQueueItf);
};

#endif //FFMPEGTEST_SLUTILS_H
