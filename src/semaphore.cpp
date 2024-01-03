/**
 * @brief 用于存放一些通用的函数
*/
#ifndef DO_ITEM_FUNCTION
#define DO_ITEM_FUNCTION

#include "semaphore.hpp"
#include "angle_solver.h"

std::mutex mat_mtx;

// 生产者通用函数
int ProduceItem(ItemRepository *ir, cv::VideoCapture& capture, int time, bool &flag) {
    ir->emptyL->wait();
    ir->stop->wait();
    ir->mtxL->wait();

    if(flag) return -1;

    Mat frame;
    capture >> frame;
    if (frame.empty()) {
        ir->mtxL->signal();
        ir->stop->signal();
        ir->fullL->signal();
        return -1;
    }
    ir->buffer[ir->in] = std::make_shared<object>();
    ir->buffer[ir->in]->type = 1; ir->buffer[ir->in]->image = frame;
    // 获取当前线程id
    std::thread::id id = std::this_thread::get_id();
    std::string res = std::string(std::to_string(std::hash<std::thread::id>{}(id)));
    ir->buffer[ir->in]->produce_id = res;

    ir->buffer[ir->in]->index = ir->in;
    ir->in = (ir->in + 1) % ir->BUFFER_SIZE;
    ir->counter++;

    ir->mtxL->signal();
    ir->stop->signal();
    ir->fullL->signal();
    return 0;
}

// 消费者通用函数
void ConsumeItem(ItemRepository *ir, ItemRepository * res, ArmorDetector* detector, int time, bool &flag) {
    ir->fullL->wait();
    ir->stop->wait(); // 在本函数最下面释放
    if(flag) return;
    ir->mtxL->wait();

    if(flag) return;

    // 获得当前指向的智能指针，不需要赋值
    // 如果为空直接return空指针
    if(ir->buffer[ir->out].get() != nullptr && ir->buffer[ir->out]->image.empty()) {
        ir->mtxL->signal();
        ir->stop->signal();
        ir->emptyL->signal();
        return;
    }
    // 取出mat
    cv::Mat src_image;
    ir->buffer[ir->out]->image.copyTo(src_image);
    std::string produce_id = std::string(ir->buffer[ir->out]->produce_id);
    ir->buffer[ir->out]->image.release();
    ir->buffer[ir->out]->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;
    // 解锁一下
    ir->mtxL->signal();
    ir->stop->signal();
    ir->emptyL->signal();
    // 如果有图片，那么传入并且识别
    pair<std::vector<cv::Point2f>, cv::Mat> detect_res = detector->DetectObjectArmor(src_image, produce_id);
    
    // 处理输出结果 通过item得到最后的识别结果res_item        
    // object* res_item = new object();
    // 下一个处理
    res->emptyL->wait();
    res->stop->wait();
    if(flag) return;
    res->mtxL->wait();

    if(flag) return;

    res->buffer[res->in] = std::make_shared<object>();

    ir->buffer[ir->out]->dst_image = detect_res.second;
    res->buffer[res->in]->type = 2; res->buffer[res->in]->dst_image = detect_res.second; res->buffer[res->in]->data = detect_res.first;
    // 打印四个点的位置
    puts("{");
    for(int i = 0;i < 4;i++){
        printf("\tPoint{%f, %f}\n", res->buffer[res->in]->data[i].x, res->buffer[res->in]->data[i].y);
    } 
    puts("}");
    // 将输出的结果放到结果队列中
    res->buffer[res->in]->index = res->in;
    res->in = (res->in + 1) % res->BUFFER_SIZE;
    res->counter++;
    // 解锁一下
    res->mtxL->signal();
    res->stop->signal(); // 释放stop
    if(flag) return;
    res->fullL->signal();
    // 返回产品地址，但是不要使用，因为怕这个数据被删除
    return; 
}

// 次级消费者通用函数
void SubConsumeItem(ItemRepository *ir, AngleSolver *solver, int time, bool& flag) {
    ir->fullL->wait();
    ir->stop->wait();
    if(flag) return;
    ir->mtxL->wait();

    if(flag) return ;

    // 获取item
    vector<double> solver_res = solver->SolveAngle(ir->buffer[ir->out]->data);

    // 创建一个字符串
    std::string res_str;
    res_str = "result {" + std::to_string(solver_res[0]) + "," + std::to_string(solver_res[1]) + "," + std::to_string(solver_res[2]) + "}";
    std::cout << res_str << std::endl;

    cv::Mat res_mat = visualizeXYZ(solver_res[0], solver_res[1], solver_res[2]);

    ir->buffer[ir->out]->res_image = res_mat;
    ir->buffer[ir->out]->result = res_str;
    ir->buffer[ir->out]->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;
    
    ir->mtxL->signal();
    ir->stop->signal();
    ir->emptyL->signal();
    // 返回产品，最好不要使用，怕其他线程删除
    return; // 返回产品.
}


cv::Mat visualizeXYZ(double x, double y, double z) {
    // 创建一个黑色图像
    cv::Mat image(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));

    // 定义立方体的六个面
    std::vector<std::vector<cv::Point3f>> cubeFaces = {
        {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}},  // 前面
        {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}},  // 后面
        {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}},  // 底部
        {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}},  // 顶部
        {{0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}},  // 左侧
        {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}}   // 右侧
    };

    // 相机内参矩阵
    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 500, 0, 250, 0, 500, 250, 0, 0, 1);

    // 畸变系数
    cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64FC1);

    // 投影并绘制立方体
    for (const auto& face : cubeFaces) {
        std::vector<cv::Point2f> projectedPoints;
        cv::projectPoints(face, cv::Mat::zeros(3, 1, CV_64FC1), cv::Mat::zeros(3, 1, CV_64FC1), cameraMatrix, distCoeffs, projectedPoints);
        std::vector<cv::Point> intPoints;
        for (const auto& p : projectedPoints) {
            intPoints.push_back(cv::Point(p.x, p.y));
        }
        cv::polylines(image, intPoints, true, cv::Scalar(255, 255, 255), 2);
    }

    // 绘制坐标轴
    std::vector<cv::Point3f> axesPoints = {
        {0, 0, 0},  // 原点
        {x, 0, 0},  // x 轴
        {0, y, 0},  // y 轴
        {0, 0, z}   // z 轴
    };
    std::vector<cv::Point2f> projectedAxesPoints;
    cv::projectPoints(axesPoints, cv::Mat::zeros(3, 1, CV_64FC1), cv::Mat::zeros(3, 1, CV_64FC1), cameraMatrix, distCoeffs, projectedAxesPoints);
    std::vector<cv::Point> intAxesPoints;
    for (const auto& p : projectedAxesPoints) {
        intAxesPoints.push_back(cv::Point(p.x, p.y));
    }
    cv::polylines(image, intAxesPoints, false, cv::Scalar(0, 0, 255), 2);

    return image;
}


#endif // !DO_ITEM_FUNCTION