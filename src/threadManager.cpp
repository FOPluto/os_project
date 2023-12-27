/**
 * 线程管理类实现cpp源文件
*/
#include "threadManager.hpp"

void ThreadManager::Produce(std::string path) {
    int base_time = (int)this->produce_time * 1e3;
    cout << path << endl;
    cv::VideoCapture capture = cv::VideoCapture(path);
    Mat frame;
    if(!capture.isOpened()){
        cout << "open video error" << endl;
        return;
    }

    puts("--------Produce Start Running--------");
    while(1) {
        capture >> frame;
        if (frame.empty()) break;
        object obj;
        obj.type = 1; obj.image = frame;
        // 获取当前线程id
        std::thread::id id = std::this_thread::get_id();
        std::string res = std::string(std::to_string(std::hash<std::thread::id>{}(id)));
        cv::imshow(res, frame);
        ProduceItem(this->mat_repository, &obj, base_time);
        waitKey(1);
    }
}

void ThreadManager::Consume() {
    puts("--------Consume Start Running--------");
    int base_time = 1000 * 1e3; // 先默认1000ms
    while(1) { // 消费者无需控制速度
        object* result = ConsumeItem(this->mat_repository, 0);
        ProduceItem(this->data_repository, result, 0);
        puts("get-put");
    }
}

void ThreadManager::SubConsume() {
    puts("------SubConsume Start Running-------");
    int base_time = (int)this->sub_consume_time * 1e3;
    while(1) {
        object* data_result = ConsumeItem(this->data_repository, base_time);
        puts("res");
    }
}

void ThreadManager::DrawProcess() {
    puts("------DrawProcess Start Running------");
}

int ThreadManager::init() {
    puts("--------Starting Initializing--------");
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
    float temp; // 临时变量一个
    fr["num_produce"] >> temp;
    this->num_produce = temp;
    fr["num_consume"] >> temp;
    this->num_consume = temp;
    cout << "num_consume: " << this->num_consume << endl;
    fr["num_sub_consume"] >> temp;
    this->num_sub_consume = temp;
    cout << "num_sub_consume: " << this->num_sub_consume << endl;
    fr["size_buffer"] >> temp;
    this->size_buffer = temp;
    cout << "size_buffer: " << this->size_buffer << endl;
    fr["size_sub_buffer"] >> temp;
    this->size_sub_buffer = temp;
    cout << "size_sub_buffer: " << this->size_sub_buffer << endl;
    fr["size_buffer"] >> temp;
    this->size_buffer = temp;
    fr["time_produce"] >> this->produce_time;
    fr["time_sub_consume"] >> this->sub_consume_time;
    cout << "size_buffer: " << this->size_buffer << endl;
    puts("--------Reading data Finished--------");
    // new两个buffer区域，这个是两个结构体，用于管理buffer区域
    this->mat_repository = new ItemRepository(0, size_buffer);
    this->data_repository = new ItemRepository(0, size_sub_buffer);
    // 创建对应数量的进程
    for(size_t i = 0;i < this->num_produce;i++) {
        std::string str = string("/root/os_project/resource/") + std::to_string(i) + std::string(".avi");
        cout << "str : " << str << endl;
        this->produce_list[i] = new std::thread(std::bind(&ThreadManager::Produce, this, (str)));
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    for(size_t i = 0;i < this->num_consume;i++) {
        this->consume_list[i] = new std::thread(&ThreadManager::Consume, this);
    }
    for(size_t i = 0;i < this->num_sub_consume;i++) {
        this->sub_consume_list[i] = new std::thread(&ThreadManager::SubConsume, this);
    }
    puts("---------Initialize Finished---------");
    return 0;
}

void ThreadManager::run() {
    puts("------------start running------------");
    for(size_t i = 0;i < this->num_produce;i++) {
        this->produce_list[i]->join();
    }
    for(size_t i = 0;i < this->num_consume;i++) {
        this->consume_list[i]->join();
    }
    for(size_t i = 0;i < this->num_sub_consume;i++) {
        this->sub_consume_list[i]->join();
    }
    puts("-----------------end-----------------");
}