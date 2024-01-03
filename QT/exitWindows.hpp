#ifndef EXITWINDOWS_HPP
#define EXITWINDOWS_HPP

#include "project.h"

class MyWindow : public QMainWindow {
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // 创建左侧标签用于显示数据
        value1Label = new QLabel(this);
        value2Label = new QLabel(this);
        value3Label = new QLabel(this);

        // 创建右侧视频播放器和视频显示部件
        videoPlayer = new QMediaPlayer(this);
        videoWidget = new QVideoWidget(this);
        videoPlayer->setVideoOutput(videoWidget);

        // 创建选择栏
        comboBox = new QComboBox(this);
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MyWindow::playSelectedVideo);

        // 创建左侧垂直布局，并将标签添加到布局中
        QVBoxLayout *leftLayout = new QVBoxLayout;
        leftLayout->addWidget(value1Label);
        leftLayout->addWidget(value2Label);
        leftLayout->addWidget(value3Label);

        // 创建右侧垂直布局，并将视频显示部件和选择栏添加到布局中
        QVBoxLayout *rightLayout = new QVBoxLayout;
        rightLayout->addWidget(videoWidget);
        rightLayout->addWidget(comboBox);

        // 创建水平布局，并将左侧和右侧布局添加到水平布局中
        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->addLayout(leftLayout);
        mainLayout->addLayout(rightLayout);

        // 创建主窗口部件，并将水平布局设置为主窗口的中心部件
        QWidget *centralWidget = new QWidget(this);
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);
    }

    // 接收并显示结构体数据的函数
    void displayData(const res_data &data) {
        // 显示数据到左侧标签
        value1Label->setText(QString("Value 1: %1").arg(*(data.num_produce)));
        value2Label->setText(QString("Value 2: %1").arg(*(data.num_consume)));
        value3Label->setText(QString("Value 3: %1").arg(*(data.num_sub_consume)));

        // 清空选择栏
        comboBox->clear();

        // 循环添加视频流名称到选择栏
        for (size_t i = 0; i < data.videoStreams.size(); ++i) {
            comboBox->addItem(QString("Video %1").arg(i + 1));
        }

        // 保存视频流数组
        videoStreams = data.videoStreams;

        // 如果有视频流，则默认播放第一个视频
        if (!videoStreams.empty()) {
            playVideo(0);
        }
    }

private slots:
    // 播放选中的视频
    void playSelectedVideo(int index) {
        if (index >= 0 && index < static_cast<int>(videoStreams.size())) {
            playVideo(index);
        }
    }

private:
    QLabel *value1Label;
    QLabel *value2Label;
    QLabel *value3Label;
    QMediaPlayer *videoPlayer;
    QVideoWidget *videoWidget;
    QComboBox *comboBox;
    std::vector<cv::VideoCapture> videoStreams;

    // 播放指定索引的视频
    void playVideo(int index) {
        if (index >= 0 && index < static_cast<int>(videoStreams.size())) {
            cv::VideoCapture &videoCapture = videoStreams[index];
            // 将 OpenCV 的 VideoCapture 绑定到 QMediaPlayer
            videoPlayer->setMedia(QMediaContent(), &videoCapture);
            videoPlayer->play();
        }
    }
};

#endif // !EXITWINDOWS_HPP
