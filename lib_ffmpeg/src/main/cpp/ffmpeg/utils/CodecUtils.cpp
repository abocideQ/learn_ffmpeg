#include "CodecUtils.h"
#include "Log.h"
#include "android/surface_texture.h"

extern "C" {

void CodecUtils::codec() {
    if (m_AVFormatContext = nullptr) m_AVFormatContext = avformat_alloc_context();
    if (avformat_open_input(&m_AVFormatContext, mUrl, NULL, NULL)) {
        LOGCATE("CodecUtils avformat_open_input error");
        return;
    }
    if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
        LOGCATE("CodecUtils avformat_find_stream_info error");
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
        LOGCATE("CodecUtils steamIndex find error");
        return;
    }
    AVCodecParameters *codec_parameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;
    m_AVCodec = avcodec_find_decoder(codec_parameters->codec_id);
    if (m_AVCodec == nullptr) {
        LOGCATE("CodecUtils avcodec_find_decoder error");
        break;
    }
    m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
}
}


