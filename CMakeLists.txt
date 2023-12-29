# 设置编译属性
set( CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_VERBOSE_MAKEFILE ON)
# 设置标准
add_definitions(-std=c++14) 
# 设置cmake版本属性
cmake_minimum_required(VERSION 2.8)
# 设置项目名称
project( os_project )
# 防止警报
if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
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
            )

# 项目内包含库
include_directories(/root/os_project/include)
# 项目引入第三方库
include_directories(/root/os_project/RobomasterCode/include)
# OpenGL自定义组件库
include_directories(/root/os_project/)
# 动态链接库
target_link_libraries( os_project 
            ${OpenCV_LIBS}
            ${OPENGL_LIBRARY} 
            ${FREEGLUT_LIBRARY} 
            ${OPENGL_LIBRARIES}
            ${GLUT_glut_LIBRARY}
            openvino::runtime
            libpthread.so
            X11
            )
# 设置标准属性
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
