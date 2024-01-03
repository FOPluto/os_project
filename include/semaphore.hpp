/**
 * @brief 用于线程管理的函数，直接在文件内实现所有函数
*/
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "armor_detector.h"
#include "angle_solver.h"
#include "project.h"

class semaphore {
    // 数量，资源可用的数量
    int count;
    // 互斥锁
    std::mutex mtk;
    // 用于唤醒线程
    std::condition_variable cv;
    
public:
    explicit semaphore(int value = 1) : count(value) {}

    void wait() {
        std::unique_lock<std::mutex> lck(mtk);
        if (--count < 0)//资源不足挂起线程
        {
            cv.wait(lck);
            g_num_waiting ++;
        }    
    }   

    void signal() {
        std::unique_lock<std::mutex> lck(mtk);
        if (++count <= 0)//有线程挂起，唤醒一个
        {
            cv.notify_one();
            g_num_waiting --;
        }    
    }
};



struct ItemRepository {
    int index;
    size_t BUFFER_SIZE; // Item buffer size.
    std::shared_ptr<object>* buffer; // 产品缓冲区
    size_t out = 0; // 消费者读取产品位置.
    size_t in = 0; // 生产者写入产品位置.
    size_t counter = 0; // 当前容量
    semaphore *mtxL;
    semaphore *emptyL;
    semaphore *fullL;
    semaphore *stop;

    ItemRepository() {

    }

    ItemRepository(int index, size_t bufferSize) : index(index), BUFFER_SIZE(bufferSize) {
        buffer = new std::shared_ptr<object> [BUFFER_SIZE]();
        emptyL = new semaphore(bufferSize);
        fullL = new semaphore(0);
        mtxL = new semaphore();
        stop = new semaphore(); // 一开始都放行
    }

    virtual ~ItemRepository() {
        delete[]buffer;
        delete mtxL;
        delete emptyL;
        delete fullL;
        delete stop;
    }
};

int ProduceItem(ItemRepository *ir, cv::VideoCapture& capture, int time, bool& flag);

void ConsumeItem(ItemRepository *ir, ItemRepository * res, ArmorDetector* detector, int time, bool &flag);

void SubConsumeItem(ItemRepository *ir, AngleSolver * solver, int time, bool& flag);

void display();

void reshape(int width, int height);

void keyboard(unsigned char key, int x, int y);

cv::Mat visualizeXYZ(double x, double y, double z);

#endif // !SEMAPHORE_H