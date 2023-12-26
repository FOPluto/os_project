/**
 * @brief 定义一个threadManager，线程管理类
 * @author 可莉不知道哦
*/

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "project.h"
#include "semaphore.hpp"

/**
 * 线程管理类
*/
class ThreadManager{
    private:
    /** 
     * 生产者数量 
    */ 
    size_t num_produce;

    /**
     * 消费者数量
    */
    size_t num_consume;

    /**
     * 次级消费者数量
    */
    size_t num_sub_consume;

    /**
     * 生产者缓冲区大小
    */
    size_t size_buffer;

    /**
     * 消费者缓冲区大小
    */
    size_t size_sub_buffer;

    /**
     * 生产者消费者次级消费者的索引
    */

    public:

    /**
     * 生产者线程函数
    */
    void Produce();

    /**
     * 消费者线程函数
    */
    void Consume();

    /**
     * 次级消费者线程函数
    */
    void SubConsume();
};


#endif // !THREADMANAGER_H
