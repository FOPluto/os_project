/**
 * @brief 用于存放一些通用的函数
*/
#ifndef DO_ITEM_FUNCTION
#define DO_ITEM_FUNCTION

#include "semaphore.hpp"
#include "angle_solver.h"

std::mutex mat_mtx;

// 生产者通用函数
int ProduceItem(ItemRepository *ir, cv::VideoCapture& capture, int time) {
    ir->emptyL->wait();
    ir->mtxL->wait();

    Mat frame;
    capture >> frame;
    if (frame.empty()) {
        ir->mtxL->signal();
        ir->fullL->signal();
        return -1;
    }
    object* obj = new object();
    obj->type = 1; obj->image = frame;
    // 获取当前线程id
    std::thread::id id = std::this_thread::get_id();
    std::string res = std::string(std::to_string(std::hash<std::thread::id>{}(id)));
    obj->produce_id = res;
    // 存到全局变量中
    // try {
    //     /* code */
    //     cv::imshow(res, frame); // imshow线程不安全
    // } catch(const std::exception& e) {
    //     std::cerr << e.what() << '\n';
    // }
    ir->buffer[ir->in] = obj;
    obj->index = ir->in;
    ir->in = (ir->in + 1) % ir->BUFFER_SIZE;
    ir->counter++;
    
    ir->mtxL->signal();
    ir->fullL->signal();
    return 0;
}

// 消费者通用函数
void ConsumeItem(ItemRepository *ir, ItemRepository * res, ArmorDetector* detector, int time) {
    ir->fullL->wait();
    ir->mtxL->wait();
    // 获得当前指向的item
    auto *item = ir->buffer[ir->out];
    // 如果为空直接return空指针
    if(item == nullptr || item->image.empty()) {
        ir->buffer[ir->out] = nullptr;
        ir->mtxL->signal();
        ir->emptyL->signal();
        return;
    }
    // 取出mat
    cv::Mat src_image;
    item->image.copyTo(src_image);
    std::string produce_id = std::string(item->produce_id);
    ir->buffer[ir->out]->image.release();
    item->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;
    // 解锁一下
    ir->mtxL->signal();
    ir->emptyL->signal();
    // 如果有图片，那么传入并且识别
    pair<std::vector<cv::Point2f>, cv::Mat> detect_res = detector->DetectObjectArmor(src_image, produce_id);
    // 处理输出结果 通过item得到最后的识别结果res_item        
    object* res_item = new object();
    item->dst_image = detect_res.second.clone();
    res_item->type = 2; res_item->dst_image = detect_res.second; res_item->data = detect_res.first;
    // 下一个处理
    res->emptyL->wait();
    res->mtxL->wait();
    // 将输出的结果放到结果队列中
    res->buffer[res->in] = res_item;
    res_item->index = res->in;
    res->in = (res->in + 1) % res->BUFFER_SIZE;
    res->counter++;
    // 打印四个点的位置
    puts("{");
    for(int i = 0;i < 4;i++) printf("\tPoint{%f, %f}\n", res_item->data[i].x, res_item->data[i].y);
    puts("}");
    // 解锁一下
    res->mtxL->signal();
    res->fullL->signal();
    // 返回产品地址，但是不要使用，因为怕这个数据被删除
    return; 
}

// 次级消费者通用函数
void SubConsumeItem(ItemRepository *ir, AngleSolver *solver, int time) {
    ir->fullL->wait();
    ir->mtxL->wait();
    // 获取item
    auto *item = ir->buffer[ir->out];
    vector<double> solver_res = solver->SolveAngle(item->data);
    ir->buffer[ir->out] = nullptr;
    item->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;

    printf("result {%f, %f, %f}\n", solver_res[0], solver_res[1], solver_res[2]);
    
    ir->mtxL->signal();
    ir->emptyL->signal();
    // 返回产品，最好不要使用，怕其他线程删除
    return; // 返回产品.
}



#endif // !DO_ITEM_FUNCTION