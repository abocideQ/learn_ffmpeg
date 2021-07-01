package lin.abcdq.ffmpeg.camera

interface CameraWrapCall {
    fun onPreview(byteArray: ByteArray, width: Int, height: Int)
    fun onCapture(byteArray: ByteArray, width: Int, height: Int)
}