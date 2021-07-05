#include "SLUtils.h"

extern "C" {

int SLUtils::slProgramCreate(SLObjectItf engineObj, SLEngineItf engineItf,
                             SLObjectItf mixerObj, SLOutputMixItf mixerItf,
                             SLObjectItf playerObj, SLPlayItf playerItf,
                             SLVolumeItf volumeItf, SLAndroidSimpleBufferQueueItf bufferQueueItf) {
    int result = createEngine(engineObj, engineItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils:: createEngine error =%d", result);
        return 0;
    }
    result = createMixer(engineItf, mixerObj, mixerItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::createMixer error =%d", result);
        return 0;
    }
    result = createPlayer(engineItf, playerObj, playerItf, mixerObj, volumeItf, bufferQueueItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::createPlayer error =%d", result);
        return 0;
    }
    return 0;
}

int SLUtils::slCallbackRegister(SLAndroidSimpleBufferQueueItf bufferQueueItf,
                                slAndroidSimpleBufferQueueCallback call, void *pContext) {
    SLresult result = (*bufferQueueItf)->RegisterCallback(bufferQueueItf, call, pContext);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::player RegisterCallback error =%d", result);
        return 0;
    }
    return result;
}

int SLUtils::createEngine(SLObjectItf engineObj, SLEngineItf engineItf) {
    SLresult result = slCreateEngine(&engineObj, 0, nullptr, 0, nullptr, nullptr);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::engine slCreateEngine error =%d", result);
        return 0;
    }
    //实例化
    result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::engine Realize error =%d", result);
        return 0;
    }
    result = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engineItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::engine GetInterface error =%d", result);
        return 0;
    }
    return result;
}

int SLUtils::createMixer(SLEngineItf engineItf, SLObjectItf mixerObj, SLOutputMixItf mixerItf) {
    const SLInterfaceID faceId[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean faceReq[1] = {SL_BOOLEAN_FALSE};
    SLresult result = (*engineItf)->CreateOutputMix(engineItf, &mixerObj, 1, faceId, faceReq);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::mixer CreateOutputMix error =%d", result);
        return 0;
    }
    result = (*mixerObj)->Realize(mixerObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::mixer Realize error =%d", result);
        return 0;
    }
    result = (*mixerObj)->GetInterface(mixerObj, SL_IID_OUTPUTMIX, &mixerItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::mixer GetInterface error =%d", result);
        return 0;
    }
    return 0;
}

int SLUtils::createPlayer(SLEngineItf engineItf, SLObjectItf playerObj, SLPlayItf playerItf,
                          SLObjectItf mixerObj, SLVolumeItf volumeItf,
                          SLAndroidSimpleBufferQueueItf bufferQueueItf) {
    SLDataLocator_AndroidBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                      2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//format pcm
            (SLuint32) 2,//channel 2
            SL_SAMPLINGRATE_44_1,//44100hz
            SL_PCMSAMPLEFORMAT_FIXED_16,//bit of sample 16
            SL_PCMSAMPLEFORMAT_FIXED_16,//container size 16
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//channel mask left right
            SL_BYTEORDER_LITTLEENDIAN //endianness
    };
    SLDataSource slDataSource = {&android_queue, &pcm};

    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, mixerObj};
    SLDataSink slDataSink = {&outputMix, nullptr};

    const SLInterfaceID faceId[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean faceReq[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult result = (*engineItf)->CreateAudioPlayer(engineItf, &playerObj, &slDataSource,
                                                      &slDataSink, 3, faceId, faceReq);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::player CreateAudioPlayer error =%d", result);
        return 0;
    }
    result = (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::player Realize error =%d", result);
        return 0;
    }
    result = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &playerItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::player GetInterface error =%d", result);
        return 0;
    }
    result = (*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE, &bufferQueueItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::player buffer GetInterface error =%d", result);
        return 0;
    }
    result = (*playerObj)->GetInterface(playerObj, SL_IID_VOLUME, &volumeItf);
    if (result != SL_RESULT_SUCCESS) {
        LOGCATE("SLUtils::player volume GetInterface error =%d", result);
        return 0;
    }
    return result;
}

//result = (*bufferQueueItf)->RegisterCallback(bufferQueueItf, AudioPlayerCallback, this);
//    if (result != SL_RESULT_SUCCESS) {
//        LOGCATE("SLUtils::player RegisterCallback error =%d", result);
//        return 0;
//    }
//void SLUtils::handlerCallBack(SLAndroidSimpleBufferQueueItf bufferQueue) {
//}
//
//void SLUtils::AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
//    SLUtils *instance = static_cast<SLUtils *>(context);
//    instance->handlerCallBack(bufferQueue);
//}
}

