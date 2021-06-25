package lin.abcdq.ffmpegtest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import lin.abcdq.ffmpeg.FFmpegJni

class MainActivity : AppCompatActivity() {

    private val mFFmpegJni = FFmpegJni()
    private lateinit var mButtonInfo: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mButtonInfo = findViewById(R.id.bt_info)
        mButtonInfo.setOnClickListener { mFFmpegJni.onInfoLog() }
    }
}