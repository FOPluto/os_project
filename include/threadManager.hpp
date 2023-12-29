/**
 * @brief 定义一个threadManager，线程管理类
 * @author 可莉不知道哦
*/

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "semaphore.hpp"
#include "UI.hpp"

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
     * 生产者队列
    */
    std::thread* produce_list[MAX_LIST_SIZE];
    /**
     * 消费者数量
    */
    size_t num_consume;
    /**
     * 消费者队列
    */
    std::thread* consume_list[MAX_LIST_SIZE];
    /**
     * 次级消费者数量
    */
    size_t num_sub_consume;
    /**
     * 次级消费者队列
    */
    std::thread* sub_consume_list[MAX_LIST_SIZE];
    /**
     * 绘制程序进程
    */
    std::thread* draw_process;
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
    ItemRepository* mat_repository, *data_repository;
    /**
     * 消费者每次运行时间，和速度成反比
    */
    float produce_time;
    /**
     * 次级消费者每次运行时间，和速度成反比 
    */
    float sub_consume_time;
    /**
     * 数据处理器角度解算器
     * 
    */
    AngleSolver *solver;
    /**
     * 模型路径
    */
    std::string bin_path, xml_path;
    /**
     * 视频源id
    */
    std::vector<std::string> source_name_list;
    /**
     * 速度map
    */
    std::unordered_map<std::string, int> speed_map;
    /**
     * 名称map
    */
    std::unordered_map<std::string, std::string> name_map;
    public:
    /**
     * 初始化函数
    */
    int init(int argc, char** argv);
    /**
     * 执行函数
    */
    void run();
    /**
     * 暂停函数
    */
    int pause();
    private:
    /**
     * 初始化绘制函数
    */
    void init_draw(int argc, char** argv);
    /**
     * 生产者线程函数
    */
    void Produce(std::string path, std::string name, int speed);
    /**
     * 消费者线程函数
    */
    void Consume();
    /**
     * 次级消费者线程函数
    */
    void SubConsume(int speed);
    /**
     * 绘制线程
    */
    void DrawProcess();
    /**
     *      用户在运行的时候可以添加生产者消费者次级消费者进程
    */
    /**
     * 添加生产者
    */
    int addProduce(std::string path, int speed);
    /**
     * 添加消费者
    */
    int addConsume();
    /**
     * 添加次级消费者
    */
    int addSubConsume(int speed);
};

#endif // !THREADMANAGER_H
