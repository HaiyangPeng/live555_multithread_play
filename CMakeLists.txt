cmake_minimum_required(VERSION 3.10)
project(RTSPStreamViewer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 OpenCV 库
find_package(OpenCV REQUIRED)

# 添加可执行文件
add_executable(RTSPStreamViewer main.cpp)

# 包含 OpenCV 头文件
include_directories(${OpenCV_INCLUDE_DIRS})

# 链接 OpenCV 库和 pthread 库
target_link_libraries(RTSPStreamViewer ${OpenCV_LIBS} pthread)
