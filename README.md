```
FFmpeg Opengles Opensles
yuv420p + nv21 + nv12 + rgb
new FFPlayer();
fun onSource(url: String)
fun onSeekTo(percent: Int)
fun onPlay()
fun onPause() 
fun onStop() 
fun onRelease()
```
![Image text](https://github.com/ABCDQ123/ffmpegtest/blob/main/lib_ffmpeg/src/main/image/player.gif)
```
Ⅰ提前编译动态库.so(把要用到的功能编译好，附带编译指定平台，方便后期使用时不用再编译并减少库的大小)

1.windows平台 + mingw(自带的msys)
  配置mingw + msys环境变量(mingw/bin + mingw/mingw32/bin +mingw/msys/bin)

2.修改FFmpeg的configure
  下载FFmpeg源代码之后，首先需要对源代码中的configure文件进行修改。
  由于编译出来的动态库文件名的版本号在.so之后（例如“libavcodec.so.5.100.1”），而android平台不能识别这样文件名，所以需要修改这种文件名。

  在configure文件中找到下面几行代码：
  SLIBNAME_WITH_MAJOR='$(SLIBNAME).$(LIBMAJOR)'
  LIB_INSTALL_EXTRA_CMD='$$(RANLIB)"$(LIBDIR)/$(LIBNAME)"'
  SLIB_INSTALL_NAME='$(SLIBNAME_WITH_VERSION)'
  SLIB_INSTALL_LINKS='$(SLIBNAME_WITH_MAJOR)$(SLIBNAME)'
  替换为下面内容就可以了：
  SLIBNAME_WITH_MAJOR='$(SLIBPREF)$(FULLNAME)-$(LIBMAJOR)$(SLIBSUF)'
  LIB_INSTALL_EXTRA_CMD='$$(RANLIB)"$(LIBDIR)/$(LIBNAME)"'
  SLIB_INSTALL_NAME='$(SLIBNAME_WITH_MAJOR)'
  SLIB_INSTALL_LINKS='$(SLIBNAME)'

3.生成类库 (编写.sh文件,通过mingw/msys执行编译)
NDK= ??????????
function build_android{
  ./configure \
  --enable-neon \
  --enable-hwaccels \
  --enable-gpl \
  --disable-postproc \
  --disable-debug \
  --enable-small \
  --enable-jni \
  --enable-mediacodec \
  --enable-decoder=h264_mediacodec \
  --disable-static \
  --enable-shared \
  --disable-doc \
  --enable-ffmpeg \
  --enable-openssl \
  --disable-ffplay \
  --disable-ffprobe \
  --disable-avdevice \
  --disable-symver \
  --enable-cross-compile \
  --target-os=android \
  --sysroot=$SYSROOT \
  --arch=$ARCH \
  --cpu=$CPU \
  --cc=$CC \
  --cxx=$CXX \
  --prefix=$PREFIX \
  --cross-prefix=$CROSS_PREFIX \
  --extra-cflags="-Os -fpic $OPTIMIZE_CFLAGS" \
  --extra-ldflags="$ADDI_LDFLAGS"
  make clean
  make
  make install
}

#arm64-v8a
PLATFORM=windows-x86_64
ARCH=arm64
CPU=armv8-a
CPP=aarch64-linux-android
API=21
# 工具链
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$PLATFORM
CC=$TOOLCHAIN/bin/$CPP$API-clang
CXX=$TOOLCHAIN/bin/$CPP$API-clang++
# 编译针对的平台
SYSROOT=$TOOLCHAIN/sysroot
#交叉编译前缀
CROSS_PREFIX=$TOOLCHAIN/bin/$CPP-
#输出路径
PREFIX=$(pwd)/android/$CPU
build_android

#armv7-a
PLATFORM=windows-x86_64
ARCH=arm
CPU=armv7-a
CPP=armv7a-linux-androideabi
API=21
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$PLATFORM
CC=$TOOLCHAIN/bin/$CPP$API-clang
CXX=$TOOLCHAIN/bin/$CPP$API-clang++
SYSROOT=$TOOLCHAIN/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/$CPP-
PREFIX=$(pwd)/android/$CPU
OPTIMIZE_CFLAGS="-mfloat-abi=softfp -mfpu=vfp -marm -march=$CPU "
build_android

6个常用功能模块
libavformat:多媒体文件或协议的封装和解封装库
libavcodec：音视频编解码库
libavfilter：音视频、字幕滤镜库
libswscale：图像格式转换库
libswresample：音频重采样库
libavutil：工具库
6常用结构体
AVFormatContext：解封装 上下文: 文件名、音视频流、时长、比特率等
AVCodecContext：编解码器 上下文: 编解码时用到的结构体、编解码器类型、视频宽高、音频通道数和采样率等
AVCodec：存储 编解码器 信息
AVStream：存储音视频 流 信息
AVPacket：存储音视频 编码 数据
AVFrame：存储音视频 解码 数据（原始数据）
```
