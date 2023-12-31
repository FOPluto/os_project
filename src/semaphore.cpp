/**
 * @brief 用于存放一些通用的函数
*/
#ifndef DO_ITEM_FUNCTION
#define DO_ITEM_FUNCTION

#include "semaphore.hpp"
#include "angle_solver.h"

// 生产者通用函数
int ProduceItem(ItemRepository *ir, cv::VideoCapture& capture, Mat& frame, int time) {
    ir->emptyL->wait();
    ir->mtxL->wait();
    //sleep维持速度
    auto start_time = std::chrono::system_clock::now();
    
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
    cv::imshow(res, frame);
    cv::waitKey(1);

    ir->buffer[ir->in] = obj;
    obj->index = ir->in;
    ir->in = (ir->in + 1) % ir->BUFFER_SIZE;
    ir->counter++;
    
    auto end_time = std::chrono::system_clock::now();
    int sleep_time = time - (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    }
    ir->mtxL->signal();
    ir->fullL->signal();
    return 0;
}

// 消费者通用函数
object *ConsumeItem(ItemRepository *ir, ItemRepository * res, ArmorDetector* detector, int time) {
    ir->fullL->wait();
    ir->mtxL->wait();
    // 维持速度

    auto start_time = std::chrono::system_clock::now();


    auto *item = ir->buffer[ir->out];
    
    // 如果为空直接return空指针
    if(item->image.empty()) {
        ir->buffer[ir->out] = nullptr;
        ir->mtxL->signal();
        ir->emptyL->signal();
        return nullptr;
    }
    cv::Mat src_image;
    item->image.copyTo(src_image);
    std::string produce_id = std::string(item->produce_id);
    ir->buffer[ir->out] = nullptr;
    item->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;

    ir->mtxL->signal();
    ir->emptyL->signal();

    // 如果有图片，那么传入并且识别
    vector<cv::Point2f> detect_res = detector->DetectObjectArmor(src_image, produce_id);
    // 处理输出结果 通过item得到最后的识别结果res_item

        
    object* res_item = new object();
    res_item->type = 2; res_item->data = detect_res;
    

    res->emptyL->wait();
    res->mtxL->wait();
    
    // 将输出的结果放到结果队列中
    res->buffer[res->in] = res_item;
    res_item->index = res->in;
    res->in = (res->in + 1) % res->BUFFER_SIZE;
    res->counter++;
    

    auto end_time = std::chrono::system_clock::now();
    int sleep_time = time - (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    }
    res->mtxL->signal();
    res->fullL->signal();
    return res_item; // 返回产品.
}

// 次级消费者通用函数
vector<double> SubConsumeItem(ItemRepository *ir, AngleSolver *solver, int time) {
    ir->fullL->wait();
    ir->mtxL->wait();
    // 维持速度
    auto start_time = std::chrono::system_clock::now();
    
    auto *item = ir->buffer[ir->out];
    
    vector<double> solver_res = solver->SolveAngle(item->data);
    ir->buffer[ir->out] = nullptr;
    item->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;
    
    auto end_time = std::chrono::system_clock::now();
    int sleep_time = time - (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    }
    ir->mtxL->signal();
    ir->emptyL->signal();
    return solver_res; // 返回产品.
}



#endif // !DO_ITEM_FUNCTION