/**
 * 线程管理类实现cpp源文件
*/
#include "threadManager.hpp"

void ThreadManager::Produce(std::string path, std::string name, int speed) {
    // 初始化速度
    std::thread::id id = std::this_thread::get_id();
    std::string current_process_id = std::string(std::to_string(std::hash<std::thread::id>{}(id)));
    this->speed_map[current_process_id] = speed * 1e3;
    cout << path << endl;
    cv::VideoCapture capture = cv::VideoCapture(path);
    Mat frame;
    if(!capture.isOpened()){
        cout << "视频读取失败，请检查路径" << endl;
        return;
    }
    puts("--------Produce Start Running--------");
    while(1) {
        if(ProduceItem(this->mat_repository, capture, frame, this->speed_map[current_process_id]) == -1) break;
    }
    puts("视频读取完毕，线程退出");
}

void ThreadManager::Consume() {
    puts("--------Consume Start Running--------");
    ArmorDetector * detector = new ArmorDetector();
    detector->InitArmor(this->xml_path, this->bin_path);
    while(1) { // 消费者无需控制速度，所以设置为0
        object* result = ConsumeItem(this->mat_repository, this->data_repository, detector, 500);
        auto res = result->data;
        puts("{");
        for(int i = 0;i < 4;i++) printf("\tPoint{%f, %f}\n", res[i].x, res[i].y);
        puts("}");
    }
    delete(detector);
}

void ThreadManager::SubConsume(int speed) {
    puts("------SubConsume Start Running-------");
    // 初始化速度
    std::thread::id id = std::this_thread::get_id();
    std::string current_process_id = std::string(std::to_string(std::hash<std::thread::id>{}(id)));
    this->speed_map[current_process_id] = speed * 1e3;
    while(1) {
        vector<double> data_result = SubConsumeItem(this->data_repository, this->solver, this->speed_map[current_process_id]);
        printf("result {%f, %f, %f}\n", data_result[0], data_result[1], data_result[2]);
    }
}

void ThreadManager::DrawProcess() {
    puts("------DrawProcess Start Running------");
    glutMainLoop(); // 进入绘制循环
}

// 初始化绘制函数
void ThreadManager::init_draw(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    drawInit(this->produce_list, this->consume_list, this->sub_consume_list,
            this->num_produce, this->num_consume, this->num_sub_consume, 
            this->mat_repository, this->data_repository, this->speed_map);
}


int ThreadManager::init(int argc, char** argv) {
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
    fr["model_bin_path"] >> this->bin_path;
    fr["model_xml_path"] >> this->xml_path;
    // 初始化角度解算器
    this->solver = new AngleSolver();
    this->solver->InitAngle();

    puts("--------Reading data Finished--------");
    // new两个buffer区域，这个是两个结构体，用于管理buffer区域
    this->mat_repository = new ItemRepository(0, size_buffer);
    this->data_repository = new ItemRepository(0, size_sub_buffer);
    // 创建对应数量的进程
    for(size_t i = 0;i < this->num_produce;i++) {
        std::string str = std::string("/root/os_project/resource/") + std::to_string(i) + std::string(".avi");
        std::string name = std::string("视频源") + std::to_string(i);
        std::cout << "创建线程id   : " << str << std::endl;
        std::cout << "创建线程name : " << name << std::endl;
        this->produce_list[i] = new std::thread(std::bind(&ThreadManager::Produce, this, str, name, this->produce_time)); // 使用默认时间初始化
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
    for(size_t i = 0;i < this->num_consume;i++) {
        this->consume_list[i] = new std::thread(&ThreadManager::Consume, this);
        this->source_name_list.push_back(string("视频源") + to_string(i));
    }
    for(size_t i = 0;i < this->num_sub_consume;i++) {
        this->sub_consume_list[i] = new std::thread(std::bind(&ThreadManager::SubConsume, this, this->sub_consume_time)); // 使用默认延迟时间初始化
    }
    puts("-----------Thread Created------------");
    this->init_draw(argc, argv);
    this->draw_process = new std::thread(&ThreadManager::DrawProcess, this);
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
    this->draw_process->join();
    puts("-----------------end-----------------");
}


// 添加函数
int addProduce(std::string path, int speed) {
    return 0;
}


int addConsume() {
    return 0;
}

int addSubConsume(int speed) {
    return 0;
}