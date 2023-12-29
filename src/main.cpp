#include "threadManager.hpp"

ThreadManager* thread_manager;

int main(int argc, char** argv){
    thread_manager = new ThreadManager();
    thread_manager->init(argc, argv);
    thread_manager->run();
    return 0;
}