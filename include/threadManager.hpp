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
     * 生产者消费者次级消费者对缓冲区的索引, 
     * 四个索引分别代表三个对象对两个缓冲区的操作
    */
    size_t idx_pro, idx_pro_con,
            idx_con_sub, idx_sub;

    /**
     * 两个缓冲区 
     * 生产者消费者用的缓冲区、消费者和次级消费者用的缓冲区
    */
    object* buffer, sub_buffer;

    public:

    /**
     * 初始化函数
    */
    int init();
    /**
     * 执行函数
    */
    void run();

    private:
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

    /**
     * 绘制线程
    */
    void DrawProcess();
};

#endif // !THREADMANAGER_H
