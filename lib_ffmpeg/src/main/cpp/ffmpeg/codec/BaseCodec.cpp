#include "BaseCodec.h"
/**
 * 解封装 + 解码
 * 1 创建解封装格式上下文
 * 2 打开输入文件 解封装
 * 3 获取音视频流
 * 4 获取音视频流索引
 * 5 获取适合的解码器
 * 6 创建解码器上下文
 * 7 打开解码器
 * 8 创建存储编码数据和解码数据的结构体
 * 9 循环解码
 * */
extern "C" {
std::mutex BaseCodec::m_Mutex;
void BaseCodec::onCodecInit(char *url, AVMediaType mediaType) {
    m_Url = url;
    m_MediaType = mediaType;
    m_Thread = new std::thread(onRunAsy, this);
}

void BaseCodec::onRunAsy(BaseCodec *ptr) {
    ptr->initCodecContext();
    ptr->videoCodec();
}

void BaseCodec::initCodecContext() {
    //avFormat + avCodec
    if (m_AVFormatContext == nullptr) m_AVFormatContext = avformat_alloc_context();
    int open_state = avformat_open_input(&m_AVFormatContext, m_Url, NULL, NULL);
    if (open_state != 0) {
        LOGCATE("videoCodec avformat_open_input error %s", m_Url);
        char errorBuffer[128];
        if (av_strerror(open_state, errorBuffer, sizeof(errorBuffer)) == 0) {
            LOGCATE("videoCodec avformat_open_input error： %s", errorBuffer);
        }
        return;
    }
    if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
        LOGCATE("videoCodec avformat_find_stream_info error");
        return;
    }
    int i = 0;
    for (; i < m_AVFormatContext->nb_streams; i++) {
        if (m_AVFormatContext->streams[i]->codecpar->codec_type == m_MediaType) {
            m_StreamIndex = i;
            break;
        }
    }
    if (m_StreamIndex == -1) {
        LOGCATE("videoCodec steamIndex find error");
        return;
    }
    AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;
    m_AVCodec = (AVCodec *) avcodec_find_decoder(codecParameters->codec_id);
    if (m_AVCodec == nullptr) {
        LOGCATE("videoCodec avcodec_find_decoder error");
        return;
    }
    m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
    if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
        LOGCATE("videoCodec: avcodec_parameters_to_context error");
        return;
    }
    AVDictionary *pAVDictionary = nullptr;
    av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);
    av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);
    av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);
    av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);
    m_Result = avcodec_open2(m_AVCodecContext, m_AVCodec, &pAVDictionary);
    if (m_Result < 0) {
        LOGCATE("videoCodec: avcodec_open2 error");
        return;
    }
    m_Packet = av_packet_alloc();
    m_Frame = av_frame_alloc();
    //avScale
    int width_video = m_AVCodecContext->width;
    int height_video = m_AVCodecContext->height;
    m_FrameScale = av_frame_alloc();
    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width_video, height_video, 1);
    m_FrameScaleBuffer = (uint8_t *) av_malloc(bufferSize * sizeof(uint8_t));
    av_image_fill_arrays(m_FrameScale->data, m_FrameScale->linesize, m_FrameScaleBuffer,
                         AV_PIX_FMT_RGBA, width_video, height_video, 1);
    m_SwsContext = sws_getContext(width_video, height_video, m_AVCodecContext->pix_fmt,
                                  width_video, height_video, AV_PIX_FMT_RGBA,
                                  SWS_FAST_BILINEAR, NULL, NULL, NULL);
}

void BaseCodec::videoCodec() {
    for (;;) {
        int result = av_read_frame(m_AVFormatContext, m_Packet);
        while (result >= 0) {
            if (m_Packet->stream_index == m_StreamIndex) {
                if (avcodec_send_packet(m_AVCodecContext, m_Packet) == AVERROR_EOF) {
                    LOGCATE("videoCodec: avcodec_send_packet error");
                    av_packet_unref(m_Packet);
                    break;
                }
                int frameCount = 0;
                while (avcodec_receive_frame(m_AVCodecContext, m_Frame) == 0) {
                    swScale();
                    frameCount++;
                }
                LOGCATE("videoCodec: frameCount %d", frameCount);
                if (frameCount > 0) {
                    break;
                }
            }
            av_packet_unref(m_Packet);
            result = av_read_frame(m_AVFormatContext, m_Packet);
        }
    }
}

void BaseCodec::swScale() {
    PixImage *image = nullptr;
    int width = m_Frame->width;
    int height = m_Frame->height;
    if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_YUV420P ||
        m_AVCodecContext->pix_fmt == AV_PIX_FMT_YUVJ420P) {
        image = PixImageUtils::pix_image_get(IMAGE_FORMAT_YUV420P, width, height, m_Frame->linesize,
                                             m_Frame->data);
        if (m_Frame->data[0] && m_Frame->data[1] && !m_Frame->data[2] &&
            m_Frame->linesize[0] == m_Frame->linesize[1] && m_Frame->linesize[2] == 0) {
            // on some android device, output of h264 mediacodec decoder is NV12 兼容某些设备可能出现的格式不匹配问题
            image->format = IMAGE_FORMAT_NV12;
            LOGCATE("yuv420 wrong , try nv12");
        }
    } else if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_NV21) {
        image = PixImageUtils::pix_image_get(IMAGE_FORMAT_NV21, width, height, m_Frame->linesize,
                                             m_Frame->data);
    } else if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_NV12) {
        image = PixImageUtils::pix_image_get(IMAGE_FORMAT_NV12, width, height, m_Frame->linesize,
                                             m_Frame->data);
    } else if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_RGBA) {
        image = PixImageUtils::pix_image_get(IMAGE_FORMAT_RGBA, width, height, m_Frame->linesize,
                                             m_Frame->data);
    } else {
        sws_scale(m_SwsContext, m_Frame->data, m_Frame->linesize, 0, m_Frame->height,
                  m_FrameScale->data, m_FrameScale->linesize);
        image = PixImageUtils::pix_image_get(IMAGE_FORMAT_RGBA, m_FrameScale->width,
                                             m_FrameScale->height, m_FrameScale->linesize,
                                             m_FrameScale->data);
    }
    SimpleRender::instance()->onBuffer(image);
}

void BaseCodec::audioCodec() {

}

void BaseCodec::swReSample() {

}

void BaseCodec::onDestroy() {
    if (m_Frame != nullptr) {
        av_frame_free(&m_Frame);
        m_Frame = nullptr;
    }
    if (m_FrameScale != nullptr) {
        av_frame_free(&m_FrameScale);
        m_FrameScale = nullptr;
    }

    if (m_FrameScaleBuffer != nullptr) {
        free(m_FrameScaleBuffer);
        m_FrameScaleBuffer = nullptr;
    }

    if (m_SwsContext != nullptr) {
        sws_freeContext(m_SwsContext);
        m_SwsContext = nullptr;
    }
    if (m_Packet != nullptr) {
        av_packet_free(&m_Packet);
        m_Packet = nullptr;
    }
    if (m_AVCodecContext != nullptr) {
        avcodec_close(m_AVCodecContext);
        avcodec_free_context(&m_AVCodecContext);
        m_AVCodecContext = nullptr;
        m_AVCodec = nullptr;
    }
    if (m_AVFormatContext != nullptr) {
        avformat_close_input(&m_AVFormatContext);
        avformat_free_context(m_AVFormatContext);
        m_AVFormatContext = nullptr;
    }
    m_Sample = nullptr;
}

BaseCodec *BaseCodec::m_Sample = nullptr;
BaseCodec *BaseCodec::instance() {
    if (m_Sample == nullptr) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_Sample == nullptr) {
            m_Sample = new BaseCodec();
        }
    }
    return m_Sample;
}
}

