#ifndef FFMPEGTEST_PIXIMAGE_H
#define FFMPEGTEST_PIXIMAGE_H

#include "Log.h"

extern "C" {
#define IMAGE_FORMAT_RGBA     1
#define IMAGE_FORMAT_NV21     2
#define IMAGE_FORMAT_NV12     3
#define IMAGE_FORMAT_YUV420P  4
typedef struct _tag_pixImage {
    int width;
    int height;
    int format;
    uint8_t *plane[3];
    int pSize[3];
    int pLineSize[3];

    _tag_pixImage() {
        width = 0;
        height = 0;
        format = 0;
        plane[0] = nullptr;
        plane[1] = nullptr;
        plane[2] = nullptr;
        pSize[0] = 0;
        pSize[1] = 0;
        pSize[2] = 0;
        pLineSize[0] = 0;
        pLineSize[1] = 0;
        pLineSize[2] = 0;
    }
} PixImage;
class PixImageUtils {
public:
    static PixImage *pix_image_get(int format, int width, int height, uint8_t *data[8]) {
        PixImage *image;
        if (width == 0) return image;
        if (height == 0) return image;
        if (format == 0) return image;
        if (data == nullptr) return image;
        image = new PixImage();
        image->width = width;
        image->height = height;
        image->format = format;
        if (image->format == IMAGE_FORMAT_YUV420P) {
            size_t size = image->width * image->height * 3.0f / 2.0f;
            image->plane[0] = data[0];
            image->plane[1] = image->plane[0] + image->width * image->height;
            image->plane[2] = image->plane[1] + (image->width * image->height / 4);
            image->pSize[0] = image->width * image->height;
            image->pSize[1] = image->width * image->height / 4;
            image->pSize[2] = image->width * image->height / 4;
            image->pLineSize[0] = image->width;
            image->pLineSize[1] = image->width / 2;
            image->pLineSize[2] = image->width / 2;
        } else if (image->format == IMAGE_FORMAT_NV21) {
            size_t size = image->width * image->height * 3.0f / 2.0f;
            image->plane[0] = data[0];
            image->plane[1] = image->plane[0] + image->width * image->height;
            image->plane[2] = nullptr;
            image->pSize[0] = image->width * image->height;
            image->pSize[1] = image->width * image->height / 2;
            image->pSize[2] = 0;
            image->pLineSize[0] = image->width;
            image->pLineSize[1] = image->width;
            image->pLineSize[2] = 0;
        } else if (image->format == IMAGE_FORMAT_NV12) {
            size_t size = image->width * image->height * 3.0f / 2.0f;
            image->plane[0] = data[0];
            image->plane[1] = image->plane[0] + image->width * image->height;
            image->plane[2] = nullptr;
            image->pSize[0] = image->width * image->height;
            image->pSize[1] = image->width * image->height / 2;
            image->pSize[2] = 0;
            image->pLineSize[0] = image->width;
            image->pLineSize[1] = image->width;
            image->pLineSize[2] = 0;
        } else if (image->format == IMAGE_FORMAT_RGBA) {
            size_t size = image->width * image->height * 4.0f;
            image->plane[0] = data[0];
            image->plane[1] = nullptr;
            image->plane[2] = nullptr;
            image->pSize[0] = image->width * image->height * 4;
            image->pSize[1] = 0;
            image->pSize[2] = 0;
            image->pLineSize[0] = image->width * 4;
            image->pLineSize[1] = 0;
            image->pLineSize[2] = 0;
        }
        return image;
    }

    static void pix_image_alloc(PixImage *image) {
        if (image->width == 0) return;
        if (image->height == 0) return;
        if (image->format == 0) return;
        if (image->format == IMAGE_FORMAT_YUV420P) {
            size_t size = image->width * image->height * 3.0f / 2.0f;
            image->plane[0] = static_cast<uint8_t *> (malloc(size));
            image->plane[1] = image->plane[0] + image->width * image->height;
            image->plane[2] = image->plane[0] + (image->width / 2) * (image->height / 2);
            image->pSize[0] = image->width * image->height;
            image->pSize[1] = image->width * image->height / 4;
            image->pSize[2] = image->width * image->height / 4;
            image->pLineSize[0] = image->width;
            image->pLineSize[1] = image->width / 2;
            image->pLineSize[2] = image->width / 2;
        } else if (image->format == IMAGE_FORMAT_NV21) {
            size_t size = image->width * image->height * 3.0f / 2.0f;
            image->plane[0] = static_cast<uint8_t *> (malloc(size));
            image->plane[1] = image->plane[0] + image->width * image->height;
            image->plane[2] = nullptr;
            image->pSize[0] = image->width * image->height;
            image->pSize[1] = image->width * image->height / 2;
            image->pSize[2] = 0;
            image->pLineSize[0] = image->width;
            image->pLineSize[1] = image->width;
            image->pLineSize[2] = 0;
        } else if (image->format == IMAGE_FORMAT_NV12) {
            size_t size = image->width * image->height * 3.0f / 2.0f;
            image->plane[0] = static_cast<uint8_t *> (malloc(size));
            image->plane[1] = image->plane[0] + image->width * image->height;
            image->plane[2] = nullptr;
            image->pSize[0] = image->width * image->height;
            image->pSize[1] = image->width * image->height / 2;
            image->pSize[2] = 0;
            image->pLineSize[0] = image->width;
            image->pLineSize[1] = image->width;
            image->pLineSize[2] = 0;
        } else if (image->format == IMAGE_FORMAT_RGBA) {
            size_t size = image->width * image->height * 4.0f;
            image->plane[0] = static_cast<uint8_t *> (malloc(size));
            image->plane[1] = nullptr;
            image->plane[2] = nullptr;
            image->pSize[0] = image->width * image->height * 4;
            image->pSize[1] = 0;
            image->pSize[2] = 0;
            image->pLineSize[0] = image->width * 4;
            image->pLineSize[1] = 0;
            image->pLineSize[2] = 0;
        }
    }

    static void pix_image_free(PixImage *image) {
        if (image == nullptr || image->plane[0] == nullptr) return;
        free(image->plane[0]);
        image->plane[0] = nullptr;
        image->plane[1] = nullptr;
        image->plane[2] = nullptr;
    }
};
}


#endif //FFMPEGTEST_PIXIMAGE_H
