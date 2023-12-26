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

using namespace cv;
using namespace std;

struct object
{
    int type;
    int idx;
    double data;
    Mat image;
    /* data */
};
