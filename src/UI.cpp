#include "UI.hpp"

static float myratio;  // angle绕y轴的旋转角，ratio窗口高宽比
static float x = 0.0f, y = 0.0f, z = 5.0f;  //相机位置
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;  //视线方向，初始设为沿着Z轴负方向

const int WIDTH = 2000;
const int HEIGHT = 1280;

const std::string name = "Demo";

bool mouseDown = false;
float xrot = 0.0f, yrot = 0.0f;
float xdiff = 0.0f, ydiff = 0.0f;

const float zoom = 0.1f;
const int width = 200;
const int height = 200;

int idx; // 显示列表
bool colorflag;
std::vector<Slider> vecSlider;

char str[] = "hello";
Button button(0, 1.5, str);
bool stop = false;

std::vector<std::string> thread_id;
std::unordered_map<std::string, int> *thread_speed;

struct bufferObj {
    ItemRepository *ir;
    float x, y, z;
    float r{}, g{}, b{};

    bufferObj(ItemRepository *ir, float x, float y, float z) : ir(ir), x(x), y(y), z(z) {
        if (!colorflag) std::srand((unsigned int) time(0)), colorflag = true;
        r = std::rand() % 10 / 10.0;
        g = std::rand() % 10 / 10.0;
        b = std::rand() % 10 / 10.0;
    }

    bufferObj(ItemRepository *ir, float x, float y, float z, float r, float g, float b)
            : ir(ir), x(x), y(y), z(z), r(r), g(g), b(b) {}
};



std::vector<bufferObj *> ghd;
std::vector<std::thread *> vt;

// 表示延迟时间ms
std::vector<float> vSpeed(10);


// 总任务配置区
void initGhd(   std::thread** producers, 
                std::thread** consumers,
                std::thread** sub_consumers,
                size_t produce_size,
                size_t consume_size,
                size_t sub_consume_size,
                ItemRepository * buffer1,
                std::vector<ItemRepository *> buffer2,
                std::unordered_map<std::string, int> *speed_map) {
    // 添加框，即缓冲区，为了显示好看，尽量奇数
    // 缓冲区标号、大小，缓冲区摆放位置
    ghd.push_back(new bufferObj(buffer1, -15 * zoom, 0, 0));
    for(size_t i = 0;i < buffer2.size(); i++){
        ghd.push_back(new bufferObj(buffer2[i], 15 * zoom, -20 * zoom * i + 9 * zoom, 0));
    }
    // ghd.push_back(new bufferObj(new ItemRepository(3, 5), 10 * zoom, -6 * zoom, 0));
    // 任务，即启动箭头任务
    // 任务名、输入缓冲区，输出缓冲区，速度
    // 延时时间 = 滑动条的值  ms
    thread_speed = speed_map;
    for(size_t i = 0;i < produce_size;i ++) {
        vt.push_back(producers[i]);
        vecSlider.emplace_back(-3.1, -0.27 + 0.5 * i, 50, 1, 1000);
        // 获取对应的线程的id, 并且赋值
        (*speed_map)[std::string(std::to_string(std::hash<std::thread::id>{}(producers[i]->get_id())))] = (int)(vecSlider[i].getVal());
        thread_id.push_back(std::string(std::to_string(std::hash<std::thread::id>{}(producers[i]->get_id()))));
    }
    for(size_t i = 0;i < consume_size * produce_size;i ++) {
        vt.push_back(consumers[i]);
        vecSlider.emplace_back(0, 1.1 - 0.3 * i - (int)(i / consume_size) * 0.3, 50, 1, 1000);
        thread_id.push_back(std::string(std::to_string(std::hash<std::thread::id>{}(consumers[i]->get_id()))));
    }    
    // 每个生产者对应一个buffer
    for(size_t i = 0;i < sub_consume_size * produce_size;i ++) {
        vt.push_back(sub_consumers[i]);
        vecSlider.emplace_back(3.1, 0.29 - 0.7 * i, 50, 1, 1000);
        // 获取对应的线程的id, 并且赋值
        (*speed_map)[std::string(std::to_string(std::hash<std::thread::id>{}(sub_consumers[i]->get_id())))] = (int)(vecSlider[i].getVal());
        thread_id.push_back(std::string(std::to_string(std::hash<std::thread::id>{}(sub_consumers[i]->get_id()))));
    }
}
   
/**
 * 定义观察方式
 */
void changeSize(int w, int h) {
    //除以0的情况
    if (h == 0) h = 1;
    myratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //设置视口为整个窗口大小
    glViewport(0, 0, w, h);
    //设置可视空间
    gluPerspective(45, myratio, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 视野漫游函数
 */
void orientMe(float directionx, float directiony) {
    x += directionx * 0.1;
    y += directiony * 0.1;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 视野漫游函数
 */
void moveMeFlat(int direction) {
    z += direction * (lz) * 0.1;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 鼠标事件
*/
void mouse(int but, int state, int x, int y) {
    if (but == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for (size_t i = 0; i < vecSlider.size(); i++) {
            vecSlider[i].listen(x, y);
            (*thread_speed)[thread_id[i]] = vecSlider[i].getVal();
        }
        if (button.listen(x, y))
            stop = !stop;
    }
    if (but == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        mouseDown = true;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else
        mouseDown = false;
}

/**
 * 鼠标移动事件
 */
void mouseMotion(int x, int y) {
    if (mouseDown) {
        yrot = x - xdiff;
        xrot = y + ydiff;
        glutPostRedisplay();
    }
}

/**
 * 加入按键控制
 */
void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            orientMe(0, 1);
            break;
        case GLUT_KEY_DOWN:
            orientMe(0, -1);
            break;
        case GLUT_KEY_LEFT:
            orientMe(-1, 0);
            break;
        case GLUT_KEY_RIGHT:
            orientMe(1, 0);
            break;
        case GLUT_KEY_PAGE_DOWN:
            moveMeFlat(-1);
            break;
        case GLUT_KEY_PAGE_UP:
            moveMeFlat(1);
            break;
        default:
            break;
    }
}

void drawSphere(ItemRepository *ir, object *ob, int i) {
    glColor3f(ob->r, ob->g, ob->b);
    glPushMatrix();
    glTranslatef(0, (i - int(ir->BUFFER_SIZE) / 2) * zoom * 2, zoom);
    if(!ob->image.empty() && ob->type == 1 || !ob->dst_image.empty() && ob->type == 2) {
        glutSolidSphere(zoom, 100, 100);
    }
    glPopMatrix();
    if(ob->type == 1) {
        if(!ob->image.empty()) drawImage(ob->image, zoom * -2.5, (i - int(ir->BUFFER_SIZE) / 2) * zoom * 2, zoom);
        if(!ob->dst_image.empty()){
            drawImage(ob->dst_image, zoom * 1.7, (i - int(ir->BUFFER_SIZE) / 2) * zoom * 2, zoom);
        } 
    } else {
        if(!ob->dst_image.empty()) drawImage(ob->dst_image, zoom * -2.5, (i - int(ir->BUFFER_SIZE) / 2) * zoom * 2, zoom);
    }
}

void drawArrow() {
    glTranslatef(0, 0, zoom);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glutSolidCone(zoom, zoom * 2, 100, 100);
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

    // 实现鼠标旋转的核心
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    for (auto &slider:vecSlider) {
        glPushMatrix();
        slider.draw();
        glPopMatrix();
    }
    button.draw();

    // 绘制球
    for (auto &item:ghd) {
        glPushMatrix();
        glTranslatef(item->x, item->y, item->z);
        for (size_t i = 0; i < item->ir->BUFFER_SIZE; i++) {
            object *ob = item->ir->buffer[i];
            if (ob) drawSphere(item->ir, item->ir->buffer[i], ob->index);
        }

        glColor3f(item->r, item->g, item->b);
        {
            glPushMatrix();
            glTranslatef(-5.7 * zoom, (int(item->ir->in) - int(item->ir->BUFFER_SIZE) / 2) * zoom * 2, 0);
            drawArrow();
            glPopMatrix();
        }
        glColor3f(item->r * 0.5, item->g * 0.5, item->b * 0.5);
        {
            glPushMatrix();
            glTranslatef(5.5 * zoom, (int(item->ir->out) - int(item->ir->BUFFER_SIZE) / 2) * zoom * 2, 0);
            drawArrow();
            glPopMatrix();
        }
        glPopMatrix();
    }

    // 最先画坐标和框
    glPushMatrix();
    glCallList(idx);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void myIdle(int i) {
    for (auto &item:ghd) item->ir->mtxL->wait();
    // 调试用的
    // std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    myDisplay();
    
    for (auto &item:ghd) item->ir->mtxL->signal();
    glutTimerFunc(20, myIdle, 1);
}


void drawInit(  std::thread** producers, 
                std::thread** consumers,
                std::thread** sub_consumers,
                size_t produce_size,
                size_t consume_size,
                size_t sub_consume_size,
                ItemRepository * buffer1,
                std::vector<ItemRepository *> buffer2,
                std::unordered_map<std::string, int> *speed_map) {
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Demo");  // 改了窗口标题

    glutDisplayFunc(myDisplay);
//    glutIdleFunc(myIdle);  // 表示在CPU空闲的时间调用某一函数
    glutTimerFunc(20, myIdle, 1);
    glutSpecialFunc(processSpecialKeys);  // 按键
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    initGhd(    producers, 
                consumers,
                sub_consumers,
                produce_size,
                consume_size,
                sub_consume_size,
                buffer1,
                buffer2,
                speed_map);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

    // 显示列表
    idx = glGenLists(1);//glGenLists()唯一的标识一个显示列表
    glNewList(idx, GL_COMPILE);//用于对显示列表进行定界。第一个参数是一个整形索引值，由glGenLists()指定

    // 框
    glColor3f(0, 1, 0);
    for (auto &item : ghd) {
        glPushMatrix();
        glTranslatef(item->x, item->y, item->z + zoom);
        glScalef(1, item->ir->BUFFER_SIZE, 1);
        glLineWidth(2);
        glutWireCube(2 * zoom);
        glPopMatrix();
    }

    // 再画线
    glPushMatrix();
    glTranslatef(-width * zoom, 0, 0);
    glColor4f(1, 0, 0, 0);
    for (int i = 0; i < 2 * width; i += 4) {
        glBegin(GL_LINES);
        glVertex3f(0, -height * zoom, 0);
        glVertex3f(0, height * zoom, 0);
        glEnd();
        glTranslatef(zoom * 4, 0, 0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -height * zoom, 0);
    glColor4f(0, 0, 1, 0);
    for (int i = 0; i < 2 * height; i += 4) {
        glBegin(GL_LINES);
        glVertex3f(-width * zoom, 0, 0);
        glVertex3f(width * zoom, 0, 0);
        glEnd();
        glTranslatef(0, zoom * 4, 0);
    }
    glPopMatrix();

    // 先画平面
    glPushMatrix();
    glTranslatef(0, 0, -0.2 * zoom);
    glColor4f(1, 1, 1, 1);
    glRectf(-width * zoom, -height * zoom, width * zoom, height * zoom);
    glPopMatrix();
    glEndList();
}

// opencv画图像
void drawImage(const cv::Mat& image, float x, float y, float z) {
    // 转换图像为 RGBA 格式
    cv::Mat rgbaImage;
    cv::cvtColor(image, rgbaImage, cv::COLOR_BGR2RGBA);

    // 创建纹理对象
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图像数据到纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaImage.cols, rgbaImage.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaImage.data);

    // 保存当前矩阵状态
    glPushMatrix();

    // 平移和缩放变换
    glTranslatef(x, y, z);
    glScalef(image.cols / 2500.0f, image.rows / 2500.0f, 1.0f);  // 缩小2400倍

    // 绘制图像
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    // 恢复矩阵状态
    glPopMatrix();

    // 清理
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureID);
    glDisable(GL_TEXTURE_2D);
}