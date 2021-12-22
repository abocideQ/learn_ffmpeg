```
Player API

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
FFMPEG编译流程

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

```
图像基础

像素：
2x2分辨率 -> 2x2个像素
RGB RGB
RGB RGB

位深：
1bit    -> 2种颜色，黑白两色
3bit    -> 红1bit 蓝1bit 绿1bit
8bit    -> 红3bit 蓝3bit 绿3bit
16bit   -> 红5bit 蓝5bit 绿6bit
24bit   -> 红8bit 蓝8bit 绿8bit
颜色数量 -> 2∧位深 -> 2∧1/2∧3/2∧8/2∧16/2∧24
   
图片格式：
JPG(JPEG) -> 有损压缩 -> 24bit
PNG -> 无损压缩 -> 8/24/32bit
GIF -> 无损压缩 -> 8bit

RBG：
红 + 蓝 + 绿
R + B + G

YUV(YCbCr)：
亮度 + 蓝色色度 + 红色色度
420采样 -> Y + UV/4
422采样 -> Y + UV/2
444采样 -> Y + UV
yuv420sp888(420) -> yyyy + u + v
NV12/NV21(420) -> yyyy + vu / yyyy + uv

转换公式：
RGB的取值范围是[0, 1]
Y的取值范围是[0, 1]
UV的取值范围是[-0.5, 0.5]
Y = 0.299R + 0.587G + 0.114B
U = 0.564(B - Y) = -0.169R - 0.331G + 0.500B
V = 0.713(R - Y) = 0.500R - 0.419G - 0.081B
R = Y + 1.403V
G = Y - 0.344U - 0.714V
B = Y + 1.770U
RGB的取值范围是[0, 255]
YUV的取值范围是[0, 255]
Y = 0.299R + 0.587G + 0.114B
U = -0.169R - 0.331G + 0.500B + 128
V = 0.500R - 0.419G - 0.081B + 128
R = Y + 1.403(V - 128)
G = Y - 0.343(U - 128) - 0.714(V - 128)
B = Y + 1.770(U - 128)
RGB的取值范围是[0,255]
Y的取值范围是[16,235]
UV的取值范围是[16,239]
Y = 0.257R + 0.504G + 0.098B + 16
U = -0.148R - 0.291G + 0.439B + 128
V = 0.439R - 0.368G - 0.071B + 128
R = 1.164(Y - 16) + 2.018(U - 128)
G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
B = 1.164(Y - 16) + 1.596(V - 128)
```
```
视频编码
常见编码
JPEG
MPEG
H.263
MPEG-4
H.264/AVC
H.265/HEVC

H264
别称 MPEG-4第10部分高级视频编码
简称 MPEG-4/AVC
原始视频 10秒+30fps+1920x1080+YUV420P -> (10*30)*(1920*1080)*1.5 = 933120000byte ≈ 889.89MB
H264压缩编码 -> ≈ 889.89MB / 10 ≈ 88M

编码
x264是一款免费的高性能的H.264开源编码器 x264编码器在FFmpeg中的名称是libx264
AVCodec *encodec = avcodec_find_encoder_by_name("libx264");
解码
FFmpeg默认已经内置了一个H.264的解码器 名称是h264
AVCodec *decodec = avcodec_find_decoder_by_name("h264");
```