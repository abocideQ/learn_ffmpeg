package lin.abcdq.ffmpegtest

import android.Manifest
import android.content.Intent
import android.graphics.Camera
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.widget.Button
import android.widget.RelativeLayout
import androidx.appcompat.app.AppCompatActivity
import lin.abcdq.ffmpeg.FFPlayer
import java.io.File

//addr2line -e libavcodec.so -f -C 000000000015fc44
class MainActivity : AppCompatActivity() {

    private val mPermissions = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.CAMERA
    )

    private var mVideoPath = ""
    private var mPlayer: FFPlayer? = null
    private lateinit var mGLContainer: RelativeLayout
    private lateinit var mButtonInfo: Button
    private lateinit var mButtonSource: Button
    private lateinit var mButtonPlay: Button
    private lateinit var mButtonPause: Button
    private lateinit var mButtonStop: Button
    private lateinit var mButtonRelease: Button


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        //权限
        requestPermissions(mPermissions, 100)
        //拷贝视频
        CAO.copyAssetsDirToSDCard(this, "video", this.obbDir.absolutePath)
        //========
        mGLContainer = findViewById(R.id.rl_content)
        mButtonInfo = findViewById(R.id.bt_info)
        mButtonInfo.setOnClickListener {
            mPlayer?.onInfoPrint()
        }
        mButtonSource = findViewById(R.id.bt_source)
        mButtonSource.setOnClickListener {
            if (mPlayer == null) {
                mPlayer = FFPlayer()
                mGLContainer.removeAllViews()
                val mGLSurfaceView = GLSurfaceView(this)
                mGLSurfaceView.setEGLContextClientVersion(3)
                mGLSurfaceView.setRenderer(mPlayer)
                mGLContainer.addView(mGLSurfaceView)
            }
//            mVideoPath = "http://vfx.mtime.cn/Video/2019/03/21/mp4/190321153853126488.mp4"
            mVideoPath = this.obbDir.absolutePath + "/video/one_piece.mp4"
            mPlayer?.onSource(mVideoPath)
        }
        mButtonPlay = findViewById(R.id.bt_play)
        mButtonPlay.setOnClickListener {
            mPlayer?.onPlay()
        }
        mButtonPause = findViewById(R.id.bt_pause)
        mButtonPause.setOnClickListener {
            mPlayer?.onPause()
        }
        mButtonStop = findViewById(R.id.bt_stop)
        mButtonStop.setOnClickListener {
            mPlayer?.onStop()
        }
        mButtonRelease = findViewById(R.id.bt_release)
        mButtonRelease.setOnClickListener {
            mPlayer?.onRelease()
            mPlayer = null
            mGLContainer.removeAllViews()
        }
    }

    fun goCamera() {
        val intent = Intent(this, CameraActivity::class.java)
        startActivity(intent)
        finish()
    }
}