#include "jni.h"
#include "Log.h"
#include "BaseCodec.h"

extern "C" {
#include <libavcodec/version.h>
#include <libavcodec/avcodec.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libavfilter/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>
};

extern "C" {

char *test = "http://vfx.mtime.cn/Video/2019/03/21/mp4/190321153853126488.mp4";
void native_ffmepg_codec(JNIEnv *env, jobject *obj) {
    BaseCodec codec = BaseCodec();
    codec.init(test, AVMEDIA_TYPE_VIDEO);
    codec.videoCodec();
}

void native_ffmpeg_onInfo(JNIEnv *env, jobject *obj) {
    LOGCATE("libavcodec %s", AV_STRINGIFY(LIBAVCODEC_VERSION));
    LOGCATE("libavformat %s", AV_STRINGIFY(LIBAVFORMAT_VERSION));
    LOGCATE("libavutil %s", AV_STRINGIFY(LIBAVUTIL_VERSION));
    LOGCATE("libavfilter %s", AV_STRINGIFY(LIBAVFILTER_VERSION));
    LOGCATE("libswresample %s", AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    LOGCATE("libswscale %s", AV_STRINGIFY(LIBSWSCALE_VERSION));
    LOGCATE("avcodec_configure %s", avcodec_configuration());
    LOGCATE("avcodec_license %s", avcodec_license());
}

const char *JNI_Class[] = {
        "lin/abcdq/ffmpeg/FFmpegJni"
};
JNINativeMethod JNI_Methods[] = {
        {"native_ffmepg_codec",  "()V", (void *) native_ffmepg_codec},
        {"native_ffmpeg_onInfo", "()V", (void *) native_ffmpeg_onInfo},
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