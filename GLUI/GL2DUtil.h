#ifndef OPENGLGAME_GL2DUTIL_H
#define OPENGLGAME_GL2DUTIL_H
#include "project.h"
#include <GL/glx.h>
#include <GL/gl.h>
// GUI主控件高度，防止同层控件绘制不清
#define ZGUI 0.01
// GUI主控件比例
#define RATE 1

struct MyPos {
    float x, y, z;
};

/**
 * 参考博客： https://blog.csdn.net/sac761/article/details/52179585
 */
MyPos screen2world(int x, int y);




// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR 128

void drawString(std::string str);


#endif //OPENGLGAME_GL2DUTIL_H
