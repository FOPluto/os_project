#include "GL2DUtil.h"

/**
 * 参考博客： https://blog.csdn.net/sac761/article/details/52179585
 */
MyPos screen2world(int x, int y) {
    int viewport[4];
    double modelview[16];
    double projection[16];
    float winX, winY, winZ;
    double posX, posY, posZ;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float) x;
    winY = (float) viewport[3] - (float) y;
    // 把已经绘制好的像素（它可能已经被保存到显卡的显存中）读取到内存
    // GL_DEPTH_COMPONENT 为深度读取
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    return {static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(posZ)};
}




// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR 128

void drawString(string str) {
    static int isFirstCall = 1;
    static GLuint lists;

    if (isFirstCall) { // 如果是第一次调用，执行初始化
        // 初始化 X Window System
        Display *display = XOpenDisplay(NULL);
        Window root = DefaultRootWindow(display);
        XVisualInfo *visualInfo;
        GLXContext glContext;

        // 创建 OpenGL 上下文并使其当前
        static int visualAttributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
        visualInfo = glXChooseVisual(display, 0, visualAttributes);
        glContext = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
        glXMakeCurrent(display, root, glContext);

        // 为每一个ASCII字符产生一个显示列表
        isFirstCall = 0;
        // 申请MAX_CHAR个连续的显示列表编号
        lists = glGenLists(MAX_CHAR);
        // 把每个字符的绘制命令都装到对应的显示列表中
        for (int i = 0; i < MAX_CHAR; ++i) {
            glNewList(lists + i, GL_COMPILE);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, i);
            glEndList();
        }
    }
    // 调用每个字符对应的显示列表，绘制每个字符
    for (int i = 0; i < str.size(); ++i)
        glCallList(lists + str[i]);
}
