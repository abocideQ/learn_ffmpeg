#ifndef FFMPEGTEST_SIMPLERENDER_H
#define FFMPEGTEST_SIMPLERENDER_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GLUtils.h"
#include "PixImage.h"
#include "Log.h"

class SimpleRender {

public:

    void onBuffer(PixImage *image);

    void onSurfaceCreated();

    void onSurfaceChanged(int w, int h);

    void onDrawFrame();

    void onDestroy();

    static SimpleRender *instance();

protected:
    PixImage *m_Image;
    int m_Width_display = 0;
    int m_Height_display = 0;

    GLuint m_VBO[4];

    GLuint m_Program;
    GLuint m_Texture[3];
    GLuint m_VAO[1];

    GLuint m_Program_Fbo_YUV420P;
    GLuint m_Program_Fbo_NV21;
    GLuint m_Program_Fbo_RGB;
    GLuint m_Texture_Fbo[1];
    GLuint m_VAO_Fbo[1];
    GLuint m_Fbo[1];
private:
    static SimpleRender *m_Sample;
};


#endif //FFMPEGTEST_SIMPLERENDER_H
