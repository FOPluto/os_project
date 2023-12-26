/**
 * 线程管理类实现cpp源文件
*/
#include "threadManager.hpp"

void ThreadManager::Produce() {
    puts("bb");
}

void ThreadManager::Consume() {
    puts("cc");
}

void ThreadManager::SubConsume() {
    puts("dd");
}

void ThreadManager::DrawProcess() {

}

int ThreadManager::init() {

    puts("---------Initialize Finished---------");
    return 0;
}

void ThreadManager::run() {
    puts("------------start running------------");

}