#include "threadManager.hpp"

ThreadManager* thread_manager;

int main(){
    thread_manager = new ThreadManager();
    thread_manager->init();
    thread_manager->run();
    return 0;
}