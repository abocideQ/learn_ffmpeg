package lin.abcdq.ffmpeg

import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class FFPlayer : GLSurfaceView.Renderer {

    init {
        System.loadLibrary("lib_ffplayer")
    }

    fun onSource(url: String) {
        native_ffplayer_onSource(url)
    }

    fun onPlay() {
        native_ffplayer_onPlay()
    }

    fun onPause() {
        native_ffplayer_onPause()
    }

    fun onStop(){
        native_ffplayer_onStop()
    }

    fun onRelease() {
        native_ffplayer_onRelease()
    }

    fun onInfoPrint() {
        native_ffplayer_onInfoPrint()
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        native_ffplayer_onSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        native_ffplayer_onSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        native_ffplayer_onDrawFrame()
    }

    private external fun native_ffplayer_onInfoPrint()

    private external fun native_ffplayer_onSource(url: String)

    private external fun native_ffplayer_onPlay()

    private external fun native_ffplayer_onPause()

    private external fun native_ffplayer_onStop()

    private external fun native_ffplayer_onRelease()

    private external fun native_ffplayer_onSurfaceCreated()

    private external fun native_ffplayer_onSurfaceChanged(width: Int, height: Int)

    private external fun native_ffplayer_onDrawFrame()


    //camera data
//    fun onCameraBuffer(byteArray: ByteArray, width: Int, height: Int) {
////        native_ffplayer_onBuffers(byteArray, width, height)
//    }
//
//    private external fun native_ffplayer_onBuffers(byteArray: ByteArray, w: Int, h: Int)

}