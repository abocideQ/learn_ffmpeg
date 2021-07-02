#ifndef FFMPEGTEST_SIMPLERENDER_H
#define FFMPEGTEST_SIMPLERENDER_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GLUtils.h"
#include "PixImage.h"
#include "Log.h"

#include <thread>
#include <mutex>

class SimpleRender {

public:

    void onBuffer(PixImage *image);

    void onSurfaceCreated();

    void onSurfaceChanged(int w, int h);

    void onDrawFrame();

    void onResume();

    void onStop();

    void onRelease();

    static SimpleRender *instance();

protected:
    //数据
    PixImage *m_Image;
    int m_Width_display = 0;
    int m_Height_display = 0;
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
private:
    static SimpleRender *m_Sample;
};


#endif //FFMPEGTEST_SIMPLERENDER_H
