#include <jni.h>
#include "SimplePlayer.h"

extern "C" {
#include <libavcodec/version.h>
#include <libavcodec/avcodec.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libavfilter/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>
}
extern "C" {
void native_ffplayer_onInfoPrint(JNIEnv *env, jobject *obj) {
    LOGCATE("libavcodec %s", AV_STRINGIFY(LIBAVCODEC_VERSION));
    LOGCATE("libavformat %s", AV_STRINGIFY(LIBAVFORMAT_VERSION));
    LOGCATE("libavutil %s", AV_STRINGIFY(LIBAVUTIL_VERSION));
    LOGCATE("libavfilter %s", AV_STRINGIFY(LIBAVFILTER_VERSION));
    LOGCATE("libswresample %s", AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    LOGCATE("libswscale %s", AV_STRINGIFY(LIBSWSCALE_VERSION));
    LOGCATE("avcodec_configure %s", avcodec_configuration());
    LOGCATE("avcodec_license %s", avcodec_license());
}

void native_ffplayer_onSource(JNIEnv *env, jobject *obj, jstring url) {
    char *p = (char *) env->GetStringUTFChars(url, 0);
    SimplePlayer::instance()->onSource(p);
}

void native_ffplayer_onPlay(JNIEnv *env, jobject *obj) {
    SimplePlayer::instance()->onPlay();
}

void native_ffplayer_onPause(JNIEnv *env, jobject *obj) {
    SimplePlayer::instance()->onPause();
}

void native_ffplayer_onStop(JNIEnv *env, jobject *obj) {
    SimplePlayer::instance()->onStop();
}

void native_ffplayer_onRelease(JNIEnv *env, jobject *obj) {
    SimplePlayer::instance()->onRelease();
}

void native_ffplayer_onSurfaceCreated(JNIEnv *env, jobject *obj) {
    SimplePlayer::instance()->onSurfaceCreated();
}

void native_ffplayer_onSurfaceChanged(JNIEnv *env, jobject *obj, jint width, jint height) {
    SimplePlayer::instance()->onSurfaceChanged(width, height);
}

void native_ffplayer_onDrawFrame(JNIEnv *env, jobject *obj) {
    SimplePlayer::instance()->onDrawFrame();
}

//camera data
//void native_ffplayer_onBuffers(JNIEnv *env, jobject *obj, jbyteArray dataBytes, jint w, jint h) {
//    int length = env->GetArrayLength(dataBytes);
//    uint8_t *buffer = new uint8_t[length];
//    env->GetByteArrayRegion(dataBytes, 0, length, reinterpret_cast<jbyte *>(buffer));
//    VideoRender::instance()->onBuffers(buffer, w, h);
//}

const char *JNI_Class[] = {
        "lin/abcdq/ffmpeg/FFPlayer"
};
JNINativeMethod JNI_Methods[] = {
        {"native_ffplayer_onInfoPrint",      "()V",                   (void *) native_ffplayer_onInfoPrint},
        {"native_ffplayer_onSource",         "(Ljava/lang/String;)V", (void *) native_ffplayer_onSource},
        {"native_ffplayer_onPlay",           "()V",                   (void *) native_ffplayer_onPlay},
        {"native_ffplayer_onPause",          "()V",                   (void *) native_ffplayer_onPause},
        {"native_ffplayer_onStop",           "()V",                   (void *) native_ffplayer_onStop},
        {"native_ffplayer_onRelease",        "()V",                   (void *) native_ffplayer_onRelease},
        {"native_ffplayer_onSurfaceCreated", "()V",                   (void *) native_ffplayer_onSurfaceCreated},
        {"native_ffplayer_onSurfaceChanged", "(II)V",                 (void *) native_ffplayer_onSurfaceChanged},
        {"native_ffplayer_onDrawFrame",      "()V",                   (void *) native_ffplayer_onDrawFrame}
};
#define JNI_LENGTH(n) (sizeof(n) / sizeof(n[0]))
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    jclass clazz = env->FindClass(JNI_Class[0]);
    if (env->RegisterNatives(clazz, JNI_Methods, JNI_LENGTH(JNI_Methods)) != JNI_OK) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
};