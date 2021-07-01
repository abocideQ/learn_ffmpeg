package lin.abcdq.ffmpegtest

import android.opengl.GLSurfaceView
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import lin.abcdq.ffmpeg.FFPlayer
import lin.abcdq.ffmpeg.camera.CameraUse
import lin.abcdq.ffmpeg.camera.CameraWrapCall

class CameraActivity : AppCompatActivity() {

    private val mPlayer = FFPlayer()
    private lateinit var mCamera: CameraUse
    private lateinit var mGLSurfaceView: GLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_camera)
        mGLSurfaceView = findViewById(R.id.gls_content)
        mGLSurfaceView.setEGLContextClientVersion(3)
        mGLSurfaceView.setRenderer(mPlayer)

        mCamera = CameraUse(this)
        mCamera.setCall(object : CameraWrapCall {
            override fun onPreview(byteArray: ByteArray, width: Int, height: Int) {
//                mPlayer.onCameraBuffer(byteArray, width, height)
            }

            override fun onCapture(byteArray: ByteArray, width: Int, height: Int) {
            }
        })
        mCamera.open()
    }
}