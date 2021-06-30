package lin.abcdq.ffmpegtest

import android.Manifest
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.os.Environment
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import lin.abcdq.ffmpeg.FFPlayer

class MainActivity : AppCompatActivity() {

    val yuv420p = "http://vfx.mtime.cn/Video/2019/03/21/mp4/190321153853126488.mp4"
    private val mVideoPath =
        Environment.getExternalStorageDirectory().absolutePath + "/byteflow/one_piece.mp4"

    private val mPlayer = FFPlayer()
    private lateinit var mButtonInfo: Button
    private lateinit var mGLSurfaceView: GLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermissions(mPermissions, 100)
        CAO.copyAssetsDirToSDCard(this, "video", this.obbDir.absolutePath)
        mButtonInfo = findViewById(R.id.bt_info)
        mButtonInfo.setOnClickListener { mPlayer.onSource(mVideoPath) }
        mGLSurfaceView = findViewById(R.id.gls_content)
        mGLSurfaceView.setEGLContextClientVersion(3)
        mGLSurfaceView.setRenderer(mPlayer)
    }

    private val mPermissions = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE
    )
}