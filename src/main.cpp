#include "threadManager.hpp"
#include "mainWindows.hpp"

ThreadManager* thread_manager;

int main(int argc, char** argv){
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    // thread_manager = new ThreadManager();
    // thread_manager->init(argc, argv);
    // thread_manager->run();
    // delete(thread_manager);
    return app.exec();
}

#include "main.moc"
