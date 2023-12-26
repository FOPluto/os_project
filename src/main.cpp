#include "threadManager.hpp"

ThreadManager thread_manager;

int main(){
    thread_manager.Produce();
    thread_manager.Consume();
    thread_manager.SubConsume();
    return 0;
}