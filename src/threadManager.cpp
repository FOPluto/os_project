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
        if(ProduceItem(this->mat_repository, capture, frame, base_time) == -1) break;
    }
}

void ThreadManager::Consume() {
    puts("--------Consume Start Running--------");
    while(1) { // 消费者无需控制速度
        object* result = ConsumeItem(this->mat_repository, this->data_repository, this->yolo_detector, 0);
        auto res = result->data;
        puts("{");
        for(int i = 0;i < 4;i++) printf("\tPoint{%f, %f}\n", res[i].x, res[i].y);
        puts("}");
    }
}

void ThreadManager::SubConsume() {
    puts("------SubConsume Start Running-------");
    int base_time = (int)this->sub_consume_time * 1e3;
    while(1) {
        vector<double> data_result = SubConsumeItem(this->data_repository, this->solver, base_time);
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("用户界面");
    
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
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
    // 初始化模型
    this->yolo_detector = new ArmorDetector();
    this->yolo_detector->InitArmor(this->xml_path, this->bin_path);
    // 初始化角度解算器
    this->solver = new AngleSolver();
    this->solver->InitAngle();
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
        this->source_name_list.push_back(string("视频源") + to_string(i));
    }
    for(size_t i = 0;i < this->num_sub_consume;i++) {
        this->sub_consume_list[i] = new std::thread(&ThreadManager::SubConsume, this);
    }
    puts("-----------Thread Created------------");
    this->init_draw(argc, argv);
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


// 添加函数
int addProduce(std::string path) {
    return 0;
}


int addConsume() {
    return 0;
}

int addSubConsume() {
    return 0;
}