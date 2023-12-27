#ifndef DO_ITEM_FUNCTION
#define DO_ITEM_FUNCTION

#include "semaphore.hpp"

void ProduceItem(ItemRepository *ir, object *item, int time) {
    ir->emptyL->wait();
    ir->mtxL->wait();
    //sleep维持速度
    auto start_time = std::chrono::system_clock::now();
    ir->buffer[ir->in] = item;
    item->index = ir->in;
    ir->in = (ir->in + 1) % ir->BUFFER_SIZE;
    ir->counter++;
    auto end_time = std::chrono::system_clock::now();
    int sleep_time = time - (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    }
    ir->mtxL->signal();
    ir->fullL->signal();
}

object *ConsumeItem(ItemRepository *ir, int time) {
    ir->fullL->wait();
    ir->mtxL->wait();
    // 维持速度
    auto start_time = std::chrono::system_clock::now();
    auto *item = ir->buffer[ir->out];
    ir->buffer[ir->out] = nullptr;
    item->index = ir->out;
    ir->out = (ir->out + 1) % ir->BUFFER_SIZE;
    ir->counter--;
    auto end_time = std::chrono::system_clock::now();
    int sleep_time = time - (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    }
    ir->mtxL->signal();
    ir->emptyL->signal();
    return item; // 返回产品.
}

#endif // !DO_ITEM_FUNCTION