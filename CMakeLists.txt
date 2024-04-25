cmake_minimum_required(VERSION 3.0)
project(EdgeDetection)

# 指定使用 Clang 编译器
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# 查找 OpenCV 库
find_package(OpenCV REQUIRED)

# 添加可执行文件
add_executable(edge_detection edge.cpp)

# 链接 OpenCV 库
target_link_libraries(edge_detection ${OpenCV_LIBS})
