package lin.abcdq.ffmpeg

class FFmpegJni {

    init {
        System.loadLibrary("ffmpeg_test")
    }

    fun onInfoLog() {
        native_ffmpeg_onInfo()
        native_ffmepg_codec()
    }

    private external fun native_ffmepg_codec()

    private external fun native_ffmpeg_onInfo()
}