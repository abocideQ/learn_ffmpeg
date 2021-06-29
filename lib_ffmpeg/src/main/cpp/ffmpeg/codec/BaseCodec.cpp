#include "BaseCodec.h"
#include "Log.h"

extern "C" {
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

void BaseCodec::init(char *url, AVMediaType mediaType) {
    m_Url = url;
    m_MediaType = mediaType;
}

void BaseCodec::videoCodec() {
    if (m_AVFormatContext == nullptr) m_AVFormatContext = avformat_alloc_context();
    avformat_network_init();
    int open_state = avformat_open_input(&m_AVFormatContext, m_Url, NULL, NULL);
    if (open_state < 0) {
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
    AVCodecParameters *codec_parameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;
    m_AVCodec = (AVCodec *) avcodec_find_decoder(codec_parameters->codec_id);
    if (m_AVCodec == nullptr) {
        LOGCATE("videoCodec avcodec_find_decoder error");
        return;
    }
    m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
    if (avcodec_parameters_to_context(m_AVCodecContext, codec_parameters) != 0) {
        LOGCATE("videoCodec: avcodec_parameters_to_context error");
        return;
    }
    m_Result = avcodec_open2(m_AVCodecContext, m_AVCodec, NULL);
    if (m_Result < 0) {
        LOGCATE("videoCodec: avcodec_open2 error");
        return;
    }
    m_Packet = av_packet_alloc();
    m_Frame = av_frame_alloc();
    while (av_read_frame(m_AVFormatContext, m_Packet) >= 0) {
        if (m_Packet->stream_index == m_StreamIndex) {
            if (avcodec_send_packet(m_AVCodecContext, m_Packet) != 0) {
                LOGCATE("videoCodec: avcodec_send_packet error");
                return;
            }
            while (avcodec_receive_frame(m_AVCodecContext, m_Frame) == 0) {
                swScale();
            }
        }
        av_packet_unref(m_Packet);
    }
    destroy();
}

void BaseCodec::swScale() {
    PixImage *image = nullptr;
    if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_YUV420P) {
        image = PixImageUtils::pix_image_get(IMAGE_FORMAT_YUV420P, m_Frame->width,
                                             m_Frame->height, m_Frame->data);
    } else if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_NV21) {
        image = PixImageUtils::pix_image_get(AV_PIX_FMT_NV21, m_Frame->width,
                                             m_Frame->height, m_Frame->data);
    } else if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_NV12) {
        image = PixImageUtils::pix_image_get(AV_PIX_FMT_NV12, m_Frame->width,
                                             m_Frame->height, m_Frame->data);
    } else if (m_AVCodecContext->pix_fmt == AV_PIX_FMT_RGBA) {
        image = PixImageUtils::pix_image_get(AV_PIX_FMT_RGBA, m_Frame->width,
                                             m_Frame->height, m_Frame->data);
    } else {
        m_FrameScale = av_frame_alloc();
        int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_Frame->width, m_Frame->height,
                                                  1);
        m_FrameScaleBuffer = (uint8_t *) av_malloc(bufferSize * sizeof(uint8_t));
        av_image_fill_arrays(m_FrameScale->data, m_FrameScale->linesize, m_FrameScaleBuffer,
                             AV_PIX_FMT_RGBA, m_Frame->width, m_Frame->height, 1);
        m_SwsContext = sws_getContext(m_Frame->width, m_Frame->height, m_AVCodecContext->pix_fmt,
                                      m_Frame->width, m_Frame->height, AV_PIX_FMT_RGBA,
                                      SWS_FAST_BILINEAR, NULL, NULL, NULL);
        sws_scale(m_SwsContext, m_Frame->data, m_Frame->linesize, 0, m_Frame->height,
                  m_FrameScale->data, m_FrameScale->linesize);
        image = PixImageUtils::pix_image_get(AV_PIX_FMT_RGBA, m_FrameScale->width,
                                             m_FrameScale->height, m_FrameScale->data);
    }
}

void BaseCodec::audioCodec() {

}

void BaseCodec::swReSample() {

}

void BaseCodec::destroy() {
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
}

}
