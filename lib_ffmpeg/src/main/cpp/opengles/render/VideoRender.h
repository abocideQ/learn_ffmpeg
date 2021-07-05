#ifndef FFMPEGTEST_VIDEORENDER_H
#define FFMPEGTEST_VIDEORENDER_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GLUtils.h"
#include "PixImage.h"
#include "Log.h"

#include <thread>
#include <mutex>

class VideoRender {

public:

    void onBuffer(PixImage *image);

    void onSurfaceCreated();

    void onSurfaceChanged(int w, int h);

    void onDrawFrame();

    void onResume();

    void onStop();

    void onRelease();

    int m_Width_display = 0;
    int m_Height_display = 0;
protected:
    //数据
    PixImage *m_Image;

    //VBO
    GLuint m_VBO[4];
    //显示部分
    GLuint m_Program;
    GLuint m_Texture[3];
    GLuint m_VAO[1];
    //离屏部分
    GLuint m_Program_Fbo_YUV420P;
    GLuint m_Program_Fbo_NV21;
    GLuint m_Program_Fbo_RGB;
    GLuint m_Texture_Fbo[1];
    GLuint m_VAO_Fbo[1];
    GLuint m_Fbo[1];
    //互斥锁
    static std::mutex m_Mutex;
    //===
    volatile bool m_Interrupt = false;
};


#endif //FFMPEGTEST_VIDEORENDER_H
