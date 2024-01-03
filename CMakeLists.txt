# 设置编译属性
set( CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wextra -O2 -g")
set(CMAKE_VERBOSE_MAKEFILE ON)
# 设置标准
add_definitions(-std=c++14) 
# 设置cmake版本属性
cmake_minimum_required(VERSION 2.8)
# 设置项目名称
project( os_project LANGUAGES CXX )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 防止警报
if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()
# 查找FreeType库
find_package(Freetype REQUIRED)
# 添加头文件路径
include_directories(${FREETYPE_INCLUDE_DIRS})
# 引入Qt库
#需要在这里导入Qt的Network配置和在pro文件里面添加QT+=network是一样的
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED Network COMPONENTS Core)
find_package(Qt5 COMPONENTS Widgets REQUIRED) 

#这里判断QT的版本号这里适用于QT6和QT5
#moc编译在这里进行，请把含有Q_OBJECT宏定义的头文件放到这里来
if(QT_VERSION VERSION_LESS 6.0.0)
    QT5_WRAP_CPP(MOC_Files
      ./Client/inc/tcpclient.h)
    message("< Qt6.0.0")
else()
    QT6_WRAP_CPP(MOC_Files
      ./Client/inc/tcpclient.h)
    message(">= Qt6.0.0")
endif()


# OpenGL相关
find_package(OpenGL REQUIRED)  
include_directories(${OPENGL_INCLUDE_DIR})  
# 寻找OpenGL组件
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})  
find_package(OpenVINO REQUIRED)
# 加入opencv库
find_package( OpenCV REQUIRED )
# 设置一下多线程
set(BOOST_THREAD_LIBRARY boost_thread -pthread)
# 添加源文件
add_executable( os_project 
                src/main.cpp 
                src/threadManager.cpp 
                src/semaphore.cpp
                RobomasterCode/src/angle_solver.cpp
                RobomasterCode/src/yolov5.cpp
                RobomasterCode/src/Timer.cpp 
                RobomasterCode/src/armor_detector.cpp
                src/UI.cpp
                GLUI/GL2DUtil.cpp
                QT/mainWindows.hpp
                # QT/exitWindows.hpp
            )

# 项目内包含库
include_directories(/root/os_project/include)
# 项目引入第三方库
include_directories(/root/os_project/RobomasterCode/include)
# OpenGL自定义组件库
include_directories(/root/os_project/)
# OpenGL自定义QT组件库
include_directories(/root/os_project/QT)
# 动态链接库
target_link_libraries( os_project PRIVATE 
            ${OpenCV_LIBS}
            ${OPENGL_LIBRARY} 
            ${FREEGLUT_LIBRARY} 
            ${OPENGL_LIBRARIES}
            ${GLUT_glut_LIBRARY}
            ${FREETYPE_LIBRARIES}
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Network
            openvino::runtime 
            libpthread.so 
            X11
            )
# 设置标准属性
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

install(TARGETS os_project
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})