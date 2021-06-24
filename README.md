```
Ⅰ编译
1.windows平台 + mingw(自带的msys)

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

3.生成类库
NDK= ??????????
function build_android{
./configure \
--enable-neon  \
--enable-hwaccels  \
--enable-gpl   \
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
--cross-prefix=$CROSS_PREFIX \
--prefix=$PREFIX \
--extra-cflags="-Os -fpic $OPTIMIZE_CFLAGS" \
--extra-ldflags="$ADDI_LDFLAGS"
make clean
#4线程编译
make -j4
make install
}
#arm64-v8a
PLATFORM=windows-x86_64
ARCH=arm64
CPU=armv8-a
CPP=aarch64-linux-android
API=21
# 编译针对的平台
SYSROOT=$NDK/platforms/android-$API/arch-$ARCH
# 工具链
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$PLATFORM
CC=$TOOLCHAIN/bin/$CPP$API-clang
CXX=$TOOLCHAIN/bin/$CPP$API-clang++
#交叉编译前缀
CROSS_PREFIX=$TOOLCHAIN/bin/$CPP-
#输出路径
PREFIX=$(pwd)/android/$CPU
build_android
```

```
#!/bin/bash
NDK=C:/Users/Users/AppData/Local/Android/Sdk/ndk/21.4.7075529
function build_android
{
./configure \
--enable-neon  \
--enable-hwaccels  \
--enable-gpl   \
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
--cross-prefix=$CROSS_PREFIX \
--prefix=$PREFIX \
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
```
