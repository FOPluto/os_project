/**
 * @brief 用于包含需要的库，并且定义全局的变量
*/

#ifndef PROJECT_H
#define PROJECT_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QtWidgets>
#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/aruco.hpp>
#include <fstream>
#include <thread>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <inference_engine.hpp>
#include <chrono>
#include <deque>
#include <queue>
#include <exception>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <X11/Xlib.h>

// 最大列表长度
#define MAX_LIST_SIZE 1000
// 默认读取的配置文件
#define DEFUALT_XML_PATH "/root/os_project/xml_path/thread.xml"
// BUffer最大长度
#define MAX_BUFFER_SIZE 100

using namespace cv;
using namespace std;

static int g_num_waiting = 0;

// 存储变量的object
struct object
{
    int type; // 类型，用于区分第一个缓冲区还是第二个缓冲区
    size_t index; // 索引
    std::vector<cv::Point2f> data; // 数据
    cv::Mat image; // 输入的图像
    cv::Mat dst_image; // 输出的图像
    cv::Mat res_image; // 最终位置图
    std::string result;
    std::string produce_id; // 所属的名称
    float r{}, g{}, b{}; // 之后画球的时候用的颜色
    /* data */
    explicit object() {
        r = rand() % 10 / 10.0;
        g = rand() % 10 / 10.0;
        b = rand() % 10 / 10.0;
    }
};

struct res_data {
public:
    bool shut_down;
    int *num_waiting;
    std::thread **produce_list;
    std::thread **consume_list;
    std::thread **sub_consume_list;
    long long int *runtime;
    size_t *num_produce;
    size_t *num_consume;
    size_t *num_sub_consume;
    std::unordered_map<std::string, int>* speed_map;

    res_data(bool flag = false) {
        this->shut_down = flag;
    }
};

// 用于提前设置参数的结构体
struct set_data
{
    /* data */
};


#endif // !PROJECT_H