#include "threadManager.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QVBoxLayout *leftLayout;

    int produce_number;

    int produce_time;

    int consume_number;

    int consume_time;
    
    int sub_consume_number;

    int sub_consume_time;

    int mat_buffer_size;

    int data_buffer_size;

    ThreadManager * thread_manager; // 程序运行的核心

    res_data * monitor; // 性能监视器，用于后面的监视功能

    int argc;
    
    char** argv; // 传入的参数


    // 按钮
    QPushButton *resetButton;

    QPushButton *confirmButton;

    QStackedWidget *stackedWidget; // 管理三个组件
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        this->argc = 0;
        this->argv = nullptr;
        // Set window title
        setWindowTitle("Layout Example");

        // Set fixed window size
        setFixedSize(800, 600);

        // Create toolbar
        QToolBar *toolbar = addToolBar("menu");
        QAction *setParamsAction = toolbar->addAction("Settings");
        QAction *historyAction = toolbar->addAction("History");
        QAction *runMonitorAction = toolbar->addAction("Monitoring");

        // Connect signals and slots
        connect(setParamsAction, &QAction::triggered, this, &MainWindow::onSetParams);
        connect(historyAction, &QAction::triggered, this, &MainWindow::onHistory);
        connect(runMonitorAction, &QAction::triggered, this, &MainWindow::onRunMonitor);

        // 初始化管理器
        stackedWidget = new QStackedWidget(this);

        // 初始化三个界面
        stackedWidget->addWidget(init_setting_page());

        stackedWidget->addWidget(init_history_page());

        stackedWidget->addWidget(init_monitor_page());

        // 创建主布局
        QVBoxLayout *layout = new QVBoxLayout();
        
        switchPage(0);

        layout->addWidget(stackedWidget);

        // 创建主部件并设置布局
        QWidget *centralWidget = new QWidget();
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
    }

private slots:

    void switchPage(int index) {
        // 根据 index 切换页面，例如使用 QStackedWidget
        stackedWidget->setCurrentIndex(index);
    }

    // 初始化设置界面的函数
    QWidget * init_setting_page() {
        // Create main window layout
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(20, 20, 20, 20); // Add padding to the main layout


        // Create top layout
        QHBoxLayout *topLayout = new QHBoxLayout();

        // Create left layout
        leftLayout = new QVBoxLayout();
        leftLayout->setContentsMargins(0, 0, 20, 0); // Add right padding to the left layout

        // Create form layout for label and line edit pairs
        QFormLayout *formLayout = new QFormLayout();
        formLayout->setSpacing(10); // Add spacing between form elements

        // Add produce setting label
        QLabel *label_produce = new QLabel("Produce Setting");
        label_produce->setStyleSheet("font-weight: bold;"); // Add bold font style
        formLayout->addRow(label_produce);

        // Add label and spin box pairs for produce setting
        QLabel *label1 = new QLabel("Number:");
        QSpinBox *spinBox1 = new QSpinBox();
        spinBox1->setObjectName("111");
        spinBox1->setMinimum(1);
        spinBox1->setValue(1);
        formLayout->addRow(label1, spinBox1);

        QLabel *label2 = new QLabel("Speed:");
        QSpinBox *spinBox2 = new QSpinBox();
        spinBox2->setObjectName("111");
        spinBox2->setMinimum(1);
        spinBox2->setValue(1);
        formLayout->addRow(label2, spinBox2);

        // Add consume setting label
        QLabel *label_consume = new QLabel("Consume Setting");
        label_consume->setStyleSheet("font-weight: bold;");
        formLayout->addRow(label_consume);

        // Add label and spin box pairs for consume setting
        QLabel *label3 = new QLabel("Number:");
        QSpinBox *spinBox3 = new QSpinBox();
        spinBox3->setObjectName("111");
        spinBox3->setMinimum(1);
        spinBox3->setValue(1);
        formLayout->addRow(label3, spinBox3);

        QLabel *label4 = new QLabel("Speed:");
        QSpinBox *spinBox4 = new QSpinBox();
        spinBox4->setMinimum(1);
        spinBox4->setValue(1);
        formLayout->addRow(label4, spinBox4);

        // Add subconsume setting label
        QLabel *label_sub_consume = new QLabel("SubConsume Setting");
        label_sub_consume->setStyleSheet("font-weight: bold;");
        formLayout->addRow(label_sub_consume);

        // Add label and spin box pairs for subconsume setting
        QLabel *label5 = new QLabel("Number:");
        QSpinBox *spinBox5 = new QSpinBox();
        spinBox5->setMinimum(1);
        spinBox5->setValue(1);
        formLayout->addRow(label5, spinBox5);

        QLabel *label6 = new QLabel("Speed:");
        QSpinBox *spinBox6 = new QSpinBox();
        spinBox6->setMinimum(1);
        spinBox6->setValue(1);
        formLayout->addRow(label6, spinBox6);

        // Add subconsume setting label
        QLabel *label_buffer = new QLabel("Buffer Setting");
        label_buffer->setStyleSheet("font-weight: bold;");
        formLayout->addRow(label_buffer);

        // Add label and spin box pairs for subconsume setting
        QLabel *label7 = new QLabel("Buffer For Image:");
        QSpinBox *spinBox7 = new QSpinBox();
        spinBox7->setMinimum(1);
        spinBox7->setValue(1);
        formLayout->addRow(label7, spinBox7);

        QLabel *label8 = new QLabel("Buffer For Data");
        QSpinBox *spinBox8 = new QSpinBox();
        spinBox8->setMinimum(1);
        spinBox8->setValue(1);
        formLayout->addRow(label8, spinBox8);

        leftLayout->addLayout(formLayout);

        // Create vertical separator line
        QFrame *separatorLine = new QFrame();
        separatorLine->setFrameShape(QFrame::VLine);
        separatorLine->setFrameShadow(QFrame::Sunken);
        topLayout->addLayout(leftLayout);
        topLayout->addWidget(separatorLine);

        // Create right layout
        QVBoxLayout *rightLayout = new QVBoxLayout();
        rightLayout->setContentsMargins(20, 0, 0, 0); // Add left padding to the right layout

        QLabel *label_r1 = new QLabel("Right Content 1");
        QLabel *label_r2 = new QLabel("Right Content 2");
        rightLayout->addWidget(label_r1);
        rightLayout->addWidget(label_r2);

        // Create bottom layout
        QHBoxLayout *bottomLayout = new QHBoxLayout();
        bottomLayout->setSpacing(20); // Add spacing between buttons

        resetButton = new QPushButton("Close");
        confirmButton = new QPushButton("Confirm");
        bottomLayout->addWidget(resetButton);
        bottomLayout->addWidget(confirmButton);

        // 事件添加

        // 将槽函数与按钮的 clicked 信号关联
        connect(confirmButton, &QPushButton::clicked, this, &MainWindow::onConfirmButtonClicked);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);


        topLayout->addLayout(rightLayout);

        mainLayout->addLayout(topLayout);

        mainLayout->addLayout(bottomLayout);

        return centralWidget;
    }

    // 初始化监视界面的函数
    QWidget* init_monitor_page()
    {
        // 创建 QWidget 作为监视界面的主部件
        QWidget* monitorWidget = new QWidget();

        // 创建主布局
        QVBoxLayout* layout = new QVBoxLayout(monitorWidget);
        layout->setContentsMargins(20, 20, 20, 20);

        // 创建标题标签
        QLabel* titleLabel = new QLabel("Variable Monitoring");
        titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        layout->addWidget(titleLabel);

        // 创建显示变量的标签和值的布局
        QFormLayout* formLayout = new QFormLayout();

        // 添加数据项的标签和值
        QLabel* numWaitingLabel = new QLabel();
        QString numWaitingText = (monitor->num_waiting != nullptr) ? QString::number(*(monitor->num_waiting)) : "";
        formLayout->addRow("Num Waiting:", numWaitingLabel);
        numWaitingLabel->setText(numWaitingText);

        if(!monitor) {
            QLabel* errorLabel = new QLabel();
            formLayout->addRow("No Process Running...", errorLabel);
            errorLabel->setText("");
        } else {
            // QLabel* runtimeLabel = new QLabel();
            // QString runtimeText = (monitor->runtime != nullptr) ? QString::number(*(monitor->runtime)) : "";
            // formLayout->addRow("Runtime:", runtimeLabel);
            // runtimeLabel->setText(runtimeText);

            // QLabel* numProduceLabel = new QLabel();
            // QString numProduceText = (monitor->num_produce != nullptr) ? QString::number(*(monitor->num_produce)) : "";
            // formLayout->addRow("Num Produce:", numProduceLabel);
            // numProduceLabel->setText(numProduceText);

            // QLabel* numConsumeLabel = new QLabel();
            // QString numConsumeText = (monitor->num_consume != nullptr) ? QString::number(*(monitor->num_consume)) : "";
            // formLayout->addRow("Num Consume:", numConsumeLabel);
            // numConsumeLabel->setText(numConsumeText);

            // QLabel* numSubConsumeLabel = new QLabel();
            // QString numSubConsumeText = (monitor->num_sub_consume != nullptr) ? QString::number(*(monitor->num_sub_consume)) : "";
            // formLayout->addRow("Num Sub Consume:", numSubConsumeLabel);
            // numSubConsumeLabel->setText(numSubConsumeText);

            // // 添加速度映射的标签和值
            // QLabel* speedMapLabel = new QLabel("Speed Map:");
            // speedMapLabel->setStyleSheet("font-weight: bold;");
            // formLayout->addRow(speedMapLabel);

            // std::unordered_map<std::string, int>* speedMap = monitor->speed_map;
            // for (const auto& pair : *speedMap) {
            //     QString key = QString::fromStdString(pair.first);
            //     QString value = QString::number(pair.second);
            //     formLayout->addRow(key + ":", new QLabel(value));
            // }
        }

        layout->addLayout(formLayout);

        // 返回监视界面的主部件
        return monitorWidget;
    }

    // 初始化历史记录的函数
    QWidget * init_history_page() {
        // 创建 QWidget 作为历史记录页面的主部件
        QWidget* historyWidget = new QWidget();

        // 创建主布局
        QVBoxLayout* layout = new QVBoxLayout(historyWidget);
        layout->setContentsMargins(20, 20, 20, 20);

        // 创建标题标签
        QLabel* titleLabel = new QLabel("History");
        titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        layout->addWidget(titleLabel);

        // 创建历史记录列表
        QListWidget* historyListWidget = new QListWidget();
        layout->addWidget(historyListWidget);

        // 添加示例历史记录项
        for (int i = 1; i <= 10; ++i) {
            QString historyItem = "History Item " + QString::number(i);
            historyListWidget->addItem(historyItem);
        }

        // 返回历史记录页面的主部件
        return historyWidget;
    }


    void printLayoutHierarchy(QLayout* layout, QString indent = "") {

        if (!layout) {
            qDebug() << indent << "<Empty Layout>";
            return;
        }

        qDebug() << indent << layout->metaObject()->className();

        for (int i = 0; i < layout->count(); ++i) {
            QLayoutItem* item = layout->itemAt(i);
            QWidget* widget = item->widget();
            QLayout* subLayout = item->layout();

            if (widget) {
                qDebug() << indent << "- Widget:" << widget->metaObject()->className();
            } else if (subLayout) {
                qDebug() << indent << "- Layout:";
                printLayoutHierarchy(subLayout, indent + "  ");
            }
        }
    }

    // 创建槽函数
    void onConfirmButtonClicked() {
        confirmButton->setEnabled(false); // 禁用按键
        this->onSetParams(); // 更新参数
        // 处理 Confirm 按钮点击事件的逻辑
        qDebug() << "Confirm button clicked!";
        this->thread_manager = new ThreadManager();
        this->monitor = this->thread_manager->init(this->produce_number,
                                                    this->produce_time,
                                                    this->consume_number,
                                                    this->consume_time,
                                                    this->sub_consume_number,
                                                    this->sub_consume_time,
                                                    this->mat_buffer_size,
                                                    this->data_buffer_size,
                                                    DEFUALT_XML_PATH,
                                                    0,
                                                    nullptr);
        std::thread * run_process = new std::thread(&ThreadManager::run, this->thread_manager);
        run_process->detach();
    }

    void onResetButtonClicked() {
        // 处理 Reset 按钮点击事件的逻辑
        qDebug() << "Reset button clicked!";
        close();
    }

    void onSetParams() {
        switchPage(0);
        printLayoutHierarchy(leftLayout);

        QLayout* topLayout = leftLayout;
        
        QLayout* firstSubLayout = qobject_cast<QLayout*>(topLayout->itemAt(0)->layout());
        // QLayout* secondSubLayout = qobject_cast<QLayout*>(firstSubLayout->itemAt(0)->layout());
        // QLayout* formLayout = qobject_cast<QLayout*>(secondSubLayout->itemAt(0)->layout());

        // QLayout *layout = leftLayout; // 获取布局对象

        QSpinBox *b1 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(2)->widget());
        QSpinBox *b2 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(4)->widget());
        QSpinBox *b3 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(7)->widget());
        QSpinBox *b4 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(9)->widget());
        QSpinBox *b5 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(12)->widget());
        QSpinBox *b6 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(14)->widget());
        QSpinBox *b7 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(17)->widget());
        QSpinBox *b8 = qobject_cast<QSpinBox*>(firstSubLayout->itemAt(19)->widget());

        // 获取文本框的值

        produce_number = b1->value();
        produce_time = b2->value();
        consume_number = b3->value();
        consume_time = b4->value();
        sub_consume_number = b5->value();
        sub_consume_time = b6->value();
        mat_buffer_size = b7->value();
        data_buffer_size = b8->value();
    }

    void onHistory() {
        switchPage(1);
        // 历史记录功能的槽函数
        // 在这里实现历史记录的逻辑
        qDebug() << "history_list";
    }

    void onRunMonitor() {
        switchPage(2);
        // 运行监视功能的槽函数
        // 在这里实现运行监视的逻辑
        qDebug() << "monitoring";
    }
};