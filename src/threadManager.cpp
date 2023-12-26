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