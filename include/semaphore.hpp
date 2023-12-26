/**
 * @brief 用于线程管理的函数，直接在文件内实现所有函数
*/
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "project.h"

class semaphore {
    
    int count;
    
    std::mutex mtk;
    
    std::condition_variable cv;
    
public:
    explicit semaphore(int value = 1) : count(value) {}

    void wait() {
        std::unique_lock<std::mutex> lck(mtk);
        if (--count < 0)//资源不足挂起线程
            cv.wait(lck);
    }

    void signal() {
        std::unique_lock<std::mutex> lck(mtk);
        if (++count <= 0)//有线程挂起，唤醒一个
            cv.notify_one();
    }
};

#endif // !SEMAPHORE_H