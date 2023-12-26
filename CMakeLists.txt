set( CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
add_definitions(-std=c++17) 

cmake_minimum_required(VERSION 2.8)
project( os_project )

find_package(OpenGL REQUIRED)  
include_directories(${OPENGL_INCLUDE_DIR})  

find_package(GLUT REQUIRED)  
include_directories(${GLUT_INCLUDE_DIR})  

add_executable( os_project src/main.cpp src/threadManager.cpp )

target_link_libraries(os_project ${OPENGL_LIBRARY} ${FREEGLUT_LIBRARY} ${OPENGL_LIBRARY})

include_directories(/root/os_project/include)
