/**
 * 线程管理类实现cpp源文件
*/
#include "threadManager.hpp"

void ThreadManager::Produce() {
    puts("--------Produce Start Running--------");
}

void ThreadManager::Consume() {
    puts("--------Consume Start Running--------");
}

void ThreadManager::SubConsume() {
    puts("------SubConsume Start Running-------");
}

void ThreadManager::DrawProcess() {
    puts("------DrawProcess Start Running------");
}

int ThreadManager::init() {
    // 使用配置文件读取参数 修改参数方便，并且避免耦合性
    std::string xml_path = DEFUALT_XML_PATH; // 定义在project.h的宏
    cv::FileStorage fr;
    fr.open(xml_path,cv::FileStorage::READ);
    while(!fr.isOpened()){
        std::cout << "armor_xml floading failed..." << std::endl;
        fr=cv::FileStorage(xml_path, cv::FileStorage::READ);
        fr.open(xml_path, cv::FileStorage::READ);
        return -1;
    }
    // 读取对应的数据
    fr["num_produce"] >> this->num_produce;
    fr["num_consume"] >> this->num_consume;
    fr["num_sub_consume"] >> this->num_sub_consume;
    fr["size_buffer"] >> this->size_buffer;
    fr["size_sub_buffer"] >> this->size_sub_buffer;
    fr["size_buffer"] >> this->size_buffer;
    puts("--------Reading data Finished--------");
    // 创建对应数量的进程
    for(int i = 0;i < this->num_produce;i++) {
        this->produce_list[i] = new std::thread(ThreadManager::Produce);
    }
    for(int i = 0;i < this->num_consume;i++) {
        this->consume_list[i] = new std::thread(ThreadManager::Consume);
    }
    for(int i = 0;i < this->num_sub_consume;i++) {
        this->sub_consume_list[i] = new std::thread(ThreadManager::SubConsume);
    }
    puts("---------Initialize Finished---------");
    return 0;
}

void ThreadManager::run() {
    puts("------------start running------------");
}