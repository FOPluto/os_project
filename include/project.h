/**
 * @brief 用于包含需要的库，并且定义全局的变量
*/

#include <GL/glut.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// 最大列表长度
#define MAX_LIST_SIZE 1000
// 默认读取的配置文件
#define DEFUALT_XML_PATH "../xml_path/thread.xml"

using namespace cv;
using namespace std;
// 存储变量的object
struct object
{
    int type; // 类型，用于区分第一个缓冲区还是第二个缓冲区
    int idx; // 所处的索引位置
    double data; // 数据
    Mat image; // 输入的图像
    /* data */
};